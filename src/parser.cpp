// Project......: Command Line Processor (clp)
// File.........: src/parser.cpp
// Author.......: elratmacfat
// Description..:
//
#include "elrat/clp/parser.hpp"

#include <regex>

namespace elrat {
namespace clp {

// ------ Parser (Top-Level class) --------------------------------------------

Parser::Parser( std::unique_ptr<Algorithm>&& a )
: _algorithm{ std::move(a) }
{
}

Parser::Parser( Parser::Function f, const std::string& s )
: _algorithm{Parser::Algorithm::Use<AlgWrapper>(f,s)}
{
}

Parser::Error Parser::parse(const std::string& s)
{
    Parser::Error e(s);
    try {
        _data = _algorithm->parse( s, e );
        return e;
    }
    catch( std::exception& exc ) {
        e.set(exc.what());
    }
    catch(...) {
        e.set("Caught unknown exception!");
    }
    return e;
}

void Parser::clear()
{
    _data.clear();
}

std::string_view Parser::getSyntaxDescription() const 
{
    return _algorithm->syntax();
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

// ------ Parser::AlgWrapper (Concrete Strategy) ------------------------------

Parser::AlgWrapper::AlgWrapper( Parser::Function f, const std::string& s )
: _function{f}
, _syntax{s}
{

}

std::string_view Parser::AlgWrapper::syntax() const 
{
    return _syntax;
}

Parser::Data Parser::AlgWrapper::parse(const std::string& s, Parser::Error& e)
{
    Parser::Data result{};
    e.source(s);
    if (!_function)
    {
        e.set("nullptr");
        return result;
    }
    
    try {
        result = _function(s);
        e.clear();
    }
    catch(std::exception& exc) {
        e.set(exc.what());
    }
    catch(...) {
        e.set("caught unknown exception");
    }
    return result;
}

// ------ Parser::Alg20 (Algorithm, Concrete Strategy) ------------------------

const std::string Parser::Alg20::_syntax("ALG20 syntax description");

std::string_view Parser::Alg20::syntax() const 
{
    return _syntax;
}

Parser::Data Parser::Alg20::parse( const std::string& s, Parser::Error& e )
{
    Parser::Data result{};


    
    return result;
}




// ------ Parser::Alg22 (Algorithm, Concrete Strategy) ------------------------

const std::string Parser::Alg22::_syntax("ALG22 syntax description");

std::string_view Parser::Alg22::syntax() const 
{
    return _syntax;
}

Parser::Data Parser::Alg22::parse( const std::string& s, Parser::Error& e )
{
    Parser::Data result{};

    return result;
}

// ------ Parser::Error -------------------------------------------------------

Parser::Error::Error(const std::string& s)
: _source{s}
{

}

Parser::Error::operator bool() const 
{
    return static_cast<bool>(_what.size());
}

std::string_view Parser::Error::source() const 
{
    return _source;
}

std::string_view Parser::Error::what() const
{
    return _what;
}

std::string_view Parser::Error::expression() const 
{
    return _expression;
}

void Parser::Error::clear()
{
    _source.clear();
    _what.clear();
    _expression.clear();
}

void Parser::Error::source(const std::string& s)
{
    _source = s;
}

void Parser::Error::set(const std::string& w, const std::string& e)
{
    _what = w;
    _expression = e;
}

// ------------------ Free Functions ------------------------------------------

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

std::ostream& operator<<(std::ostream& os,const Parser::Error& e)
{
    if ( !e ) {
        return os;
    }
    os << "Error: \"" << e.what() << "\"\n";
    if ( e.source().size() ) {
        os << "-> source = \"" << e.source() << "\"\n";
    }
    if ( e.expression().size() ) {
        os << "-> expression =  \"" << e.expression() << "\"\n";
    }
    return os;
}

} // namespace clp
} // namespace elrat

