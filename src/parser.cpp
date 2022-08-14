#include "elrat/clp/parser.hpp"

#include <regex>
#include <stdexcept>

using namespace elrat::clp;

//
// Parser
//
Parser::Parser( std::shared_ptr<InputInterface> p )
: _result{p}
, _state{State::CMD}
{
    if ( p == nullptr ) 
        throw std::invalid_argument("Parser initialized with nullptr");
}
void Parser::parse(const std::string& chunk)
{
    if ( _state == State::LOCKED )
        throw std::runtime_error("parser is locked.");

    // Separate 'chunk' into pieces
    
    // iterate over elements

        switch (_state)
        {
        case State::CMD:
            
            break;
        case State::CMD_PARAM:
            break;
        case State::OPT_PARAM:
            break;
        case State::LOCKED:
            break;
        default:
            throw std::logic_error("Unhandled case (Parser::parse())");
        }
}

bool Parser::done() const 
{
    return (_state == State::LOCKED );
}

std::shared_ptr<InputReaderInterface> Parser::getResult()
{
    if ( *_result ) 
        _state = State::LOCKED;
    return _result;
}


