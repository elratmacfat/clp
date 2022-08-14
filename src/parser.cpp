#include "elrat/clp/parser.hpp"
#include "elrat/clp/common.hpp"

#include <regex>
#include <stdexcept>

using namespace elrat::clp;


//
// Parser
//

const std::string Parser::_syntax(
    "<cmd> [<param>] [[<--option|-o>] [<param>]*]*" );

Parser::Parser( std::shared_ptr<InputInterface> p )
: _result{p}
, _state{State::Empty}
{
    if ( p == nullptr ) 
        throw std::invalid_argument("Parser initialized with nullptr");
}

std::string_view Parser::getSyntaxDescription() const 
{
    return _syntax;
}

void Parser::parse(const std::string& chunk)
{
    if ( _state == State::Locked )
        throw std::runtime_error("parser is locked.");

    // Separate 'chunk'
    //
    // -> ""-enclosed strings won't be separated.
    // -> short hand options, like "-f", will be treated as element
    // -> separators 
    //    -> equal sign
    //    -> all white spaces
    //
    std::regex rgx("(\".+\")|(-[a-zA-Z])|([^=\\s]+)");
    auto begin = std::sregex_iterator( 
        chunk.begin(), 
        chunk.end(),
        rgx );
    auto end = std::sregex_iterator();
    std::vector<std::string> elements{};
    for( auto it = begin; it != end; it ++ )
        elements.emplace_back( it->str() );

    std::string* current_option=nullptr;
    for ( auto& e : elements )
    {
        switch (_state)
        {
        case State::Empty:
            _result->setCommand(e);
            _state=State::ParsedCommand;
            break;
        case State::ParsedCommand:
                        
            if ( is_parameter(e) )
                _result->addCommandParameter(e);
            else {
                _result->addOption(e);
                current_option = &e;
                _state=State::ParsedOption;
            }
            break;
        case State::ParsedOption:
            if ( is_parameter(e) )
                _result->addOptionParameter(*current_option, e);
            else {
                _result->addOption(e);
                current_option = &e;
            }
            break;
        case State::Locked:
            throw std::logic_error("Parser is locked.");
            break;
        default:
            throw std::logic_error("Unhandled case (Parser::parse())");
        }
    }
}

bool Parser::done() const 
{
    return (_state == State::Locked );
}

std::shared_ptr<InputReaderInterface> Parser::getResult()
{
    if ( *_result ) 
        _state = State::Locked;
    return _result;
}


