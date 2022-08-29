// Project......: Command Line Processor (clp)
// File.........: src/parser.cpp
// Author.......: elratmacfat
// Description..: 
//

#include <stdexcept>

#include <elrat/clp.hpp>

#include "parser.hpp"

using elrat::clp::CommandLine;
using elrat::clp::Parser;
using elrat::clp::ParserWrapper;
using elrat::clp::NativeParser;

Parser::~Parser()
{

}

ParserWrapper::ParserWrapper( ParsingFunction func, const std::string& desc )
: parsing_function{func}
, syntax_description{desc}
{
    
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
    CommandLine result{};
    TokenHandlerFactory factory(&result);

    State current_state{ State::Expecting_Command };
    
    auto tokenHandler{ factory.create( current_state ) };
    
    for (auto it = tokens.begin(); it != tokens.end(); ++it) 
    {
        auto next_state = tokenHandler->handle(*it);
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
    {
        return State::Received_Invalid_Token;
    }
    target->command = token;
    return State::Received_Anything_Else;
}

State TokenHandlerForCommandParameter::handle(const std::string& token)
{
    target->parameters.push_back(token);
    return State::Received_Anything_Else;
}

State TokenHandlerForOptionParameter::handle(const std::string& token)
{
    auto& opt = target->options.back();
    opt.second.push_back(token);
    return State::Received_Anything_Else;
}

State TokenHandlerForAnythingElse::handle(const std::string& token)
{
    if ( IsOptionPack(token) )
    {
        add_option_pack(this->target, token);
        return State::Received_Anything_Else;
    }
    if ( IsOption(token) )
    {
        add_option(this->target, token);
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

TokenHandlerFactory::TokenHandlerFactory(elrat::clp::CommandLine* p)
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

    default:
        throw std::logic_error("Invalid State");
    }

    return nullptr;
}


const RegEx IsIdentifierPlus("[a-zA-Z_][\\w\\-]*");
const RegEx IsOption("--[_a-zA-Z][\\w\\-]*");
const RegEx IsOptionPack("-[a-zA-Z]+");
const RegEx IsEqualSign("[=]");

void add_option(CommandLine* p, const std::string& token)
{
    static const int preceeding_dashes = 2;
    p->options.push_back( 
        std::make_pair(token.substr(preceeding_dashes), std::vector<std::string>{}) );
}

void add_option_pack(CommandLine* p, const std::string& token)
{
    for( int i{1}; i < token.size(); i++ ) // omit the single preceeding dash
    {
        p->options.push_back( 
            std::make_pair(std::string(1,token[i]), std::vector<std::string>{}) );
    }
}

