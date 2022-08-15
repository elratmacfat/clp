// Project......: Command Line Processor (clp)
// File.........: src/parser.cpp
// Author.......: elratmacfat
// Description..:
//
#include "elrat/clp/parser.hpp"

#include <regex>

namespace elrat {
namespace clp {

Parser::Parser( Strategy s )
: _strategy{s}
{
}

bool Parser::parse(const std::string& s)
{
    try {
        _data = _strategy(s);
        return true;
    }
    catch( std::exception& e ) {
        _errmsg = e.what();
    }
    catch(...) {
        _errmsg = "Caught unknown exception!";
    }
    return false;
}

void Parser::clear()
{
    _data.clear();
    _errmsg.clear();
}

std::string_view Parser::getLastError() const 
{
    return _errmsg;
}

Parser::operator bool() const
{
    return static_cast<bool>(_data.size());
}

int Parser::getCommandParameterCount() const
{
    return _data.at(0).size() - 1;
}


int Parser::getOptionCount() const
{
    if (!(*this))
        throw std::out_of_range("Out Of Range: Parser::getOptionCount()");
    return _data.size() - 1;
}

int Parser::getOptionIndex(const std::string& s) const
{
    for( int i{1}; i < _data.size(); i++ )
    {
        if( _data.at(i).at(0) == s )
            return i-1;
    }
    return 0;
}

int Parser::getOptionParameterCount(int i) const
{
    return _data.at(i+1).size() - 1;
}
    
std::string_view Parser::getCommand() const
{
    return _data.at(0).at(0);
}
    
std::string_view Parser::getCommandParameter(int i) const
{   
    return _data.at(0).at(i+1);
}
   
std::string_view Parser::getOption(int i) const 
{
    return _data.at(i+1).at(0);
}

std::string_view Parser::getOptionParameter(int i,int k) const
{
    return _data.at(i+1).at(k+1);
}

const Parser::Data& Parser::data() const 
{
    return _data;
}

Parser::Data Parser::DefaultStrategy(const std::string& s)
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

std::ostream& operator<<(std::ostream& os, const Parser& p)
{
    auto& data = p.data();
    if (!p) 
        return os;

    os << "Command...: [" << p.getCommand() << "]\n";
    for( int i{0}; i < p.getCommandParameterCount(); i++ )
    {
        os << "   #" << i << ": [" << p.getCommandParameter(0) << "]\n";
    }
    for( int i{0}; i < p.getOptionCount(); i++ )
    {
        os << "- Option..: [" << p.getOption(i) << "]\n";
        for( int k{0}; k < p.getOptionParameterCount(i); k++ ) 
        {
            os << "   #" << k << ": [" << p.getOptionParameter(i,k) << "]\n";
        }
    }
    return os;
}

} // namespace clp
} // namespace elrat

