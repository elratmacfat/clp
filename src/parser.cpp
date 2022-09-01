// Project......: Command Line Processor (clp)
// File.........: src/parser.cpp
// Author.......: elratmacfat
// Description..: 
//

#include <stdexcept>

#include "parser.hpp"
#include <elrat/clp.hpp>


using elrat::clp::CommandLine;
using elrat::clp::Parser;
using elrat::clp::ParserWrapper;
using elrat::clp::NativeParser;

//
// Implementation of private header "parser.hpp"
//
std::string to_string(State state)
{
    std::string result;
    switch(state)
    {
    case State::Expecting_Command:
        result = "Expecting command token"; break;
    case State::Received_Invalid_Token:
        result = "Received invalid token"; break;
    case State::Received_Option:
        result = "Received long option token"; break;
    case State::Received_EqualSign:
        result = "Received equal sign token"; break;
    case State::Received_Anything_Else:
        result = "Received option pack or command parameter token"; break;
    case State::Received_Option_Twice:
        result = "Received option twice"; break;
    default:
        result = "Logic error: State machine in undefined state.";
    }
    return result;
}

bool is_error(State state)
{
    if ( state == State::Received_Invalid_Token || 
        state == State::Received_Option_Twice )
    {
        return true;
    }
    return false;
}

std::vector<std::string> tokenize(const std::string& input)
{
    std::vector<std::string> result{};
    std::regex separating_regex("(\"[^\"]+\")|([^\\s=]+)|(=)");
    auto begin{ std::sregex_iterator(input.begin(), input.end(), separating_regex) };
    auto end{ std::sregex_iterator() };
    for( auto it{begin}; it!=end; it++ )
        result.push_back( it->str() );
    return result;
}

bool add_option(CommandLine* p, const std::string& option)
{
    if (p->optionExists(option))
        return false;
    p->addOption(option);
    return true;
}

bool add_long_option(CommandLine* p, const std::string& token)
{
    static const int preceeding_dashes = 2;
    auto option{ token.substr(preceeding_dashes) };
    return add_option(p,option);
}

bool add_option_pack(CommandLine* p, const std::string& token)
{
    for( int i{1}; i < token.size(); i++ ) // omit the single preceeding dash
    {
        const std::string option(1,token[i]);
        if (!add_option(p, option))
            return false;
    }
    return true;
}

void throw_runtime_error(const std::string& msg)
{
    throw std::runtime_error(msg);
}

void throw_invalid_argument(const std::string& msg)
{
    throw std::invalid_argument(msg);
}

void throw_invalid_argument(const State& state, const std::string& token)
{
    throw std::invalid_argument(
        std::string("Token [") +
        token +
        "]: " +
        to_string(state));
}

const RegEx IsIdentifierPlus(
    "[a-zA-Z_][\\w\\-_]*"
);

const RegEx IsOption(
    "--[_a-zA-Z][\\w\\-]*"
);

const RegEx IsOptionPack(
    "-[a-zA-Z]+"
);

const RegEx IsEqualSign(
    "[=]"
);


TokenHandler::TokenHandler(CommandLine* p)
: target{p}
{
    // done
}

TokenHandler::~TokenHandler()
{
    // done
}

State TokenHandlerExpectingCommand::handle(const std::string& token)
{
    if ( !IsIdentifierPlus(token) )
        return State::Received_Invalid_Token;
    target->setCommand(token);
    return State::Received_Anything_Else;
}

State TokenHandlerForCommandParameter::handle(const std::string& token)
{
    if (IsEqualSign(token))
        return State::Received_Invalid_Token;
    target->addCommandParameter(token);
    return State::Received_Anything_Else;
}

State TokenHandlerForOptionParameter::handle(const std::string& token)
{
    if (IsEqualSign(token))
        return State::Received_Invalid_Token;
    target->addOptionParameter(token);
    return State::Received_Anything_Else;
}

State TokenHandlerForAnythingElse::handle(const std::string& token)
{
    if ( IsOptionPack(token) )
    {
        if (add_option_pack(this->target, token))
            return State::Received_Anything_Else;
        return State::Received_Option_Twice;
    }
    if ( IsOption(token) )
    {
        if( add_long_option(this->target, token))
            return State::Received_Option;
        return State::Received_Option_Twice;
    }
    return TokenHandlerForCommandParameter::handle(token);
}

State TokenHandlerOnOptionReception::handle(const std::string& token)
{
    if (IsEqualSign(token))
        return State::Received_EqualSign;
    return TokenHandlerForAnythingElse::handle(token);
}


TokenHandlerFactory::TokenHandlerFactory(CommandLine* p)
: target{p}
{
}

std::unique_ptr<TokenHandler> TokenHandlerFactory::createTokenHandler(State state)
{
    switch(state)
    {
    case State::Expecting_Command:
        return std::make_unique<TokenHandlerExpectingCommand>(target);

    case State::Received_Option:
        return std::make_unique<TokenHandlerOnOptionReception>(target);

    case State::Received_EqualSign:
        return std::make_unique<TokenHandlerForOptionParameter>(target);

    case State::Received_Anything_Else:
        return std::make_unique<TokenHandlerForAnythingElse>(target);
    
    case State::Received_Invalid_Token:
    case State::Received_Option_Twice:
        break;
    }
    throw_runtime_error(to_string(state));
    return std::unique_ptr<TokenHandler>{nullptr};
}

//
// Implementation of public header "clp.hpp"
//

Parser::~Parser()
{
    // virtual destructor
}

ParserWrapper::ParserWrapper( ParsingFunction func, const std::string& desc )
: parsing_function{func}
, syntax_description{desc}
{
    if (!parsing_function)
        throw_runtime_error("Parsing Function cannot be nullptr.");
}

CommandLine ParserWrapper::parse(const std::string& input) const
{
    if ( parsing_function )
        return parsing_function(input);
    return CommandLine{};
}

const std::string& ParserWrapper::getSyntaxDescription() const
{
    return syntax_description;
}

const std::string NativeParser::SyntaxDescription(
    "<command> "
    "[-<option-pack>] "
    "[--<option> [ = <option-parameter>]] "
    "[<command-parameter>]"); 

CommandLine NativeParser::parse(const std::string& input) const 
{
    auto tokens{ tokenize(input) };
    if ( !tokens.size() )
        throw_invalid_argument("Empty input");

    CommandLine result{};
    TokenHandlerFactory factory(&result);

    State current_state{ State::Expecting_Command };
    
    auto tokenHandler{ factory.createTokenHandler( current_state ) };
    
    for (auto it = tokens.begin(); it != tokens.end(); ++it) 
    {
        auto next_state = tokenHandler->handle(*it);
        if ( is_error(next_state) )
        {
            throw_invalid_argument(next_state,*it);
        }
        if ( next_state != current_state ) 
        {
            tokenHandler = factory.createTokenHandler(next_state);
            current_state = next_state;
        }
    }
    return result;
}

const std::string& NativeParser::getSyntaxDescription() const 
{
    return SyntaxDescription;
}

