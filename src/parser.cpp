// Project......: Command Line Processor (clp)
// File.........: src/parser.cpp
// Author.......: elratmacfat
// Description..: Implementation of parser interface's nested classes
//                - data 
//                - error
//
//                Implementation of the library's own parser
//                - native_parser
//
//                Implementation of the parser wrapper
//                - parser_wrapper
//
#include "elrat/clp/parser.hpp"

#include <algorithm>
#include <regex>

using namespace elrat;

//-----------------------------------------------------------------------------
//
// data
//
//

clp::parser::data::data( clp::parser::data::structure&& s )
: _s{ std::move(s) }
{
}

bool clp::parser::data::empty() const
{
    return ( _s.size() == 0 );
}

clp::parser::data::operator bool() const 
{
    return (_s.size() > 0 );
}

int clp::parser::data::cmd_param_count() const
{
    return _s.at(0).size() - 1;
}


int clp::parser::data::opt_count() const
{
    if (this->empty())
        throw std::out_of_range("Out Of Range: parser::data::opt_count()");
    return _s.size() - 1;
}

int clp::parser::data::opt_exists(const std::string& s) const
{
    for( int i{1}; i < _s.size(); i++ )
    {
        if( _s.at(i).at(0) == s )
            return i;
    }
    return 0;
}

int clp::parser::data::opt_param_count(int i) const
{
    return _s.at(i).size() - 1;
}
    
std::string_view clp::parser::data::cmd() const
{
    return _s.at(0).at(0);
}
    
std::string_view clp::parser::data::cmd_param(int i) const
{   
    return _s.at(0).at(i);
}
   
std::string_view clp::parser::data::opt(int i) const 
{
    return _s.at(i).at(0);
}

std::string_view clp::parser::data::opt_param(int i,int k) const
{
    return _s.at(i).at(k);
}

//-----------------------------------------------------------------------------
// 
// error
//
//

clp::parser::error::error(
    code c,
    const std::string& msg,
    const std::string& src )
: _code{c}
, _msg{msg}
, _src{src}
{

}

clp::parser::error::operator bool() const 
{
    return ( _msg.size() > 0 );
}

clp::parser::error::code clp::parser::error::operator()() const
{
    return _code;
}

std::string_view clp::parser::error::message() const
{
    return _msg;
}

std::string_view clp::parser::error::source() const 
{
    return _src;
}

//-----------------------------------------------------------------------------
//
// parser_wrapper
//
//

clp::parser_wrapper::parser_wrapper(function f, const std::string& s)
: _function{f}
, _syntax{s}
{

}

clp::parser::data clp::parser_wrapper::parse(const std::string& s, error& err)
{
    try {
        return _function(s,err);
    }
    catch( std::exception& exc ) {
        err = error( error::code::exception, exc.what() );
    }
    catch(...) {
        err = error( error::code::exception, "unknown" );
    }
    return data(); // default constructed empty data object.
}

std::string_view clp::parser_wrapper::syntax() const 
{
    return _syntax;
}

//-----------------------------------------------------------------------------
//
// native_parser
//
//

clp::parser::data clp::native_parser::parse(
    const std::string& s, 
    error& err)
{
    auto rgxmatch{[](const std::string& token, const char rgx[]) {
        return std::regex_match(token,std::regex(rgx));
    }};
    auto is_identplus{[&](const std::string& token ) {
        return rgxmatch(token,"^([a-zA-Z_][\\w\\-]*)$");
    }};
    auto is_opt{[&](const std::string& token){
        return rgxmatch(token,"^(--[_a-zA-Z][\\w\\-]*)$");
    }};
    auto is_opt_pack{[&](const std::string& token){
        return rgxmatch(token,"^(-[a-zA-Z]+)$");
    }};
    auto is_equal_sign{[&](const std::string& token){
        return rgxmatch(token,"^([=])$");
    }};
    auto add_opt{[](data::structure& d, const std::string& token){
        auto ss = token.substr(2,token.size());
        for( auto it = d.begin()+1; it != d.end(); it++ )
            if ( it->at(0) == ss )
                return false;
        d.push_back({ss});
        return true;
    }};
    auto add_opt_pack{[](data::structure& d, const std::string& token) {
        for( auto it = token.begin() + 1; it != token.end(); it++ ) {
            std::string ss(1,*it);
            for ( auto it2 = d.begin()+1; it2 != d.end(); it2++ )
                if ( it2->at(0) == ss )
                    return false;
            d.push_back({ss});
        }
        return true;
    }};
    auto add_opt_param{[](data::structure& d, const std::string& token) {
        d.back().push_back(token);
    }};
    auto add_param{[](data::structure& d, const std::string& token) {
        d.at(0).push_back(token);
    }};

    using data = parser::data;

    data::structure raw_data{};
    
    if ( !s.size() ) 
    {
        err = error(error::code::no_input );
        return data();
    }

    std::regex rgx("(\".+\")|([^\\s=]+)|(=)");
    auto begin{std::sregex_iterator( s.begin(), s.end(), rgx )};
    auto end{std::sregex_iterator()};

    std::vector<std::string> elements;
    for( auto it{begin}; it!=end; it++ )
        elements.push_back( it->str() );

    if ( !elements.size() )
    {
        err = error(error::code::failure);
        return data();
    }

    int state{0};
    for ( auto& e : elements )
    {
        switch ( state )
        {
        // First iteration. Accept 'command'
        case 0:
            if ( !is_identplus(e) ) 
            {
                err = error(error::code::syntax, e, s );
                return data();
            }    
            raw_data.push_back({e});
            state = 1;
            break;
        
        // Previously added 'command', a 'parameter' or an 'optionpack'
        // Accepting 'option', 'optionpack' and 'parameter'
        case 1:
            if ( is_opt(e) ) 
            {
                if (!add_opt(raw_data,e)) {
                    err = error(error::code::redundant, e, s );
                    return data();
                }
                state = 2;
            }
            else if ( is_opt_pack(e) )
            {
                if (!add_opt_pack(raw_data,e)) {
                    err = error(error::code::redundant, e, s);
                    return data();
                }
                state = 1;
            }
            else // parameter
            {
                if ( is_equal_sign(e) )
                {
                    err = error(error::code::syntax, e, s);
                    return data();
                }
                raw_data.at(0).push_back(e);
                state = 1;
            }
            break;
        
        // a single option preceeded by a double-dash was the previous token
        // accepting equal sign, command parameter or options
        case 2: 
            if ( is_equal_sign(e) )
            {
                state = 3;
            }
            else if ( is_opt(e) ) 
            {
                if (!add_opt(raw_data,e)) {
                    err = error( error::code::redundant, e, s );
                    return data();
                }
                state = 2;
            }
            else if ( is_opt_pack(e) )
            {
                if (!add_opt_pack(raw_data,e)) {
                    err = error(error::code::redundant, e, s );
                    return data();
                }
                state = 1;
            }
            else 
            {
                raw_data.at(0).push_back(e);
                state = 1;
            }
            break;

        // option assignment (=) was previous. Now accepting 'parameter'
        case 3:
            raw_data.back().push_back( e );
            state = 1;
            break;

        default:
            err = error(error::code::syntax, e, s );
            return data();
        }
    }
    

    return data( std::move(raw_data) );
}

std::string_view clp::native_parser::syntax() const 
{
    return _syntax;
}

const std::string clp::native_parser::_syntax(
    "<cmd> "
    "[-<option-pack>] "
    "[--<long-option> [ = <option-parameter>]] "
    "[<cmd-parameter>]"); 

