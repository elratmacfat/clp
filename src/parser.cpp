// Project......: Command Line Processor (clp)
// File.........: src/parser.cpp
// Author.......: elratmacfat
// Description..:
//
#include "elrat/clp/parser.hpp"

#include <algorithm>
#include <regex>

using namespace elrat;

clp::parser::error::error(
    const std::string& msg,
    const std::string& xpr,
    const std::string& src )
: _msg{msg}
, _xpr{xpr}
, _src{src}
{

}

clp::parser::error::operator bool() const 
{
    return ( _msg.size() > 0 );
}

std::string_view clp::parser::error::source() const 
{
    return _src;
}

std::string_view clp::parser::error::message() const
{
    return _msg;
}

std::string_view clp::parser::error::expression() const 
{
    return _xpr;
}

clp::parser_wrapper::parser_wrapper( 
    clp::parser_wrapper::function f,
    const std::string& s )
: _function{f}
, _syntax{s}
{

}

clp::data clp::parser_wrapper::parse(
    const std::string& s, 
    clp::parser::error& err)
{
    try {
        return _function(s,err);
    }
    catch( std::exception& exc ) {
        err = clp::parser::error( exc.what() );
    }
    catch(...) {
        err = parser::error("parser_wrapper::parse() -> unknown exception");
    }
    return clp::data(); // default constructed empty data object.
}

std::string_view clp::parser_wrapper::syntax() const 
{
    return _syntax;
}

clp::data clp::default_parser::parse(
    const std::string& s, 
    clp::parser::error& err)
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
    auto add_opt{[](clp::data::structure& d, const std::string& token){
        auto ss = token.substr(2,token.size());
        for( auto it = d.begin()+1; it != d.end(); it++ )
            if ( it->at(0) == ss )
                return false;
        d.push_back({ss});
        return true;
    }};
    auto add_opt_pack{[](clp::data::structure& d, const std::string& token) {
        for( auto it = token.begin() + 1; it != token.end(); it++ ) {
            std::string ss(1,*it);
            for ( auto it2 = d.begin()+1; it2 != d.end(); it2++ )
                if ( it2->at(0) == ss )
                    return false;
            d.push_back({ss});
        }
        return true;
    }};
    auto add_opt_param{[](clp::data::structure& d, const std::string& token) {
        d.back().push_back(token);
    }};
    auto add_param{[](clp::data::structure& d, const std::string& token) {
        d.at(0).push_back(token);
    }};

    clp::data::structure raw_data{};
    
    if ( !s.size() )
        return clp::data();

    std::regex rgx("(\".+\")|([^\\s=]+)|(=)");
    auto begin{std::sregex_iterator( s.begin(), s.end(), rgx )};
    auto end{std::sregex_iterator()};

    std::vector<std::string> elements;
    for( auto it{begin}; it!=end; it++ )
        elements.push_back( it->str() );

    if ( !elements.size() )
        throw std::runtime_error("Failed parsing input.");

    int state{0};
    for ( auto& e : elements )
    {
        switch ( state )
        {
        // First iteration. Accept 'command'
        case 0:
            if ( !is_identplus(e) ) 
            {
                err = clp::parser::error("Invalid 'command' token", e, s );
                return clp::data();
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
                    err = clp::parser::error("option already configured");
                    return clp::data();
                }
                state = 2;
            }
            else if ( is_opt_pack(e) )
            {
                if (!add_opt_pack(raw_data,e)) {
                    err = clp::parser::error("option already configured");
                    return clp::data();
                }
                state = 1;
            }
            else // parameter
            {
                if ( is_equal_sign(e) )
                {
                    err = clp::parser::error("Illegal token.", e, s );
                    return clp::data();
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
                    err = clp::parser::error("option already configured");
                    return clp::data();
                }
                state = 2;
            }
            else if ( is_opt_pack(e) )
            {
                if (!add_opt_pack(raw_data,e)) {
                    err = clp::parser::error("option already configured");
                    return clp::data();
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
            err = clp::parser::error("Invalid token", e, s );
            return clp::data();
        }
    }
    

    return clp::data( std::move(raw_data) );
}

std::string_view clp::default_parser::syntax() const 
{
    return _syntax;
}

const std::string clp::default_parser::_syntax(
    "<cmd> "
    "[-<option-pack>] "
    "[--<long-option> [ = <option-parameter>]] "
    "[<cmd-parameter>]"); 


/*
Parser::Data Parser::LegacyFunction(const std::string& s)
{
    auto is_option = [](const std::string& s) {
        return std::regex_match( s, std::regex("^(--?[_a-zA-Z]+)$"));
                    
    };
    
    auto is_identifier = [](const std::string& s) {
        return std::regex_match( s, std::regex("^([_a-zA-Z]+)$"));
    };

    if ( !s.size() )
        return Data();

    std::regex rgx("(\".+\")|([^=\\s]+)");
    auto begin{std::sregex_iterator( s.begin(), s.end(), rgx )};
    auto end{std::sregex_iterator()};

    std::vector<std::string> elements;
    for( auto it{begin}; it!=end; it++ )
        elements.push_back( it->str() );

    if ( !elements.size() )
        throw std::runtime_error("Failed parsing input.");

    if ( !is_identifier(elements[0]))
        throw std::invalid_argument("First expression must be an identifier");
    
    Data result;
    result.push_back( { elements[0] } );
    for( int i{1}; i < elements.size(); i++ )
    {
        if ( is_option(elements[i]) )
            result.push_back( { elements[i] } );
        else
            result.back().push_back( elements[i] );
    }
    return result;
}
*/

