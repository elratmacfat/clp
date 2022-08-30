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

const RegEx IsIdentifierPlus("[a-zA-Z_][\\w\\-_]*");
const RegEx IsOption("--[_a-zA-Z][\\w\\-]*");
const RegEx IsOptionPack("-[a-zA-Z]+");
const RegEx IsEqualSign("[=]");

bool option_exists(ModifiableCommandLine* p, const std::string& option)
{
    return p->optionExists(option);
}

void add_option(ModifiableCommandLine* p, const std::string& option)
{
    if (option_exists( p, option ))
        throw_invalid_token();
    p->addOption(option);
}

void add_long_option(ModifiableCommandLine* p, const std::string& token)
{
    static const int preceeding_dashes = 2;
    auto option{ token.substr(preceeding_dashes) };
    add_option(p,option);
}

void add_option_pack(ModifiableCommandLine* p, const std::string& token)
{
    for( int i{1}; i < token.size(); i++ ) // omit the single preceeding dash
    {
        const std::string option(1,token[i]);
        add_option(p, option);
    }
}

void throw_invalid_argument(const char* msg)
{
    throw std::invalid_argument(msg);
}

void throw_invalid_token()
{
    throw std::invalid_argument("Invalid Token");
}

void throw_invalid_state()
{
    throw std::invalid_argument("Invalid State");
}


TokenHandler::TokenHandler(ModifiableCommandLine* p)
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
    {
        return State::Received_Invalid_Token;
    }
    target->setCommand(token);
    return State::Received_Anything_Else;
}

State TokenHandlerForCommandParameter::handle(const std::string& token)
{
    target->addParameter(token);
    return State::Received_Anything_Else;
}

State TokenHandlerForOptionParameter::handle(const std::string& token)
{
    target->addOptionParameter(token);
    return State::Received_Anything_Else;
}

State TokenHandlerForAnythingElse::handle(const std::string& token)
{
    if ( IsEqualSign(token) )
    {   
        return State::Received_EqualSign;
    }
    if ( IsOptionPack(token) )
    {
        add_option_pack(this->target, token);
        return State::Received_Anything_Else;
    }
    if ( IsOption(token) )
    {
        add_long_option(this->target, token);
        return State::Received_Option;
    }
    return TokenHandlerForCommandParameter::handle(token);
}

State TokenHandlerOnOptionReception::handle(const std::string& token)
{
    if ( IsEqualSign(token) )
    {
        return State::Received_EqualSign;
    }
    return TokenHandlerForAnythingElse::handle(token);
}


std::vector<std::string> tokenize(const std::string& input)
{
    std::vector<std::string> result{};
    std::regex separating_regex("(\".+\")|([^\\s=]+)|(=)");
    auto begin{ std::sregex_iterator(input.begin(), input.end(), separating_regex) };
    auto end{ std::sregex_iterator() };
    for( auto it{begin}; it!=end; it++ )
        result.push_back( it->str() );
    return result;
}

TokenHandlerFactory::TokenHandlerFactory(ModifiableCommandLine* p)
: target{p}
{
}

std::unique_ptr<TokenHandler> TokenHandlerFactory::create(State state)
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
        throw_invalid_token();
    default:
        throw_invalid_state();
    }
    return std::unique_ptr<TokenHandler>{};
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
        throw_invalid_argument("Parsing Function cannot be nullptr.");
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

    ModifiableCommandLine result{};
    TokenHandlerFactory factory(&result);

    State current_state{ State::Expecting_Command };
    
    auto tokenHandler{ factory.create( current_state ) };
    
    for (auto it = tokens.begin(); it != tokens.end(); ++it) 
    {
        auto next_state = tokenHandler->handle(*it);
        if ( next_state == State::Received_EqualSign && 
            current_state != State::Received_Option )
        {
            throw_invalid_token();
        }
        if ( next_state != current_state ) 
        {
            tokenHandler = factory.create(next_state);
            current_state = next_state;
        }
    }
    return result;
}

const std::string& NativeParser::getSyntaxDescription() const 
{
    return SyntaxDescription;
}

