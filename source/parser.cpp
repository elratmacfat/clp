#include "elrat/clp.hpp"
#include "parser.hpp"

#include <stdexcept>

using elrat::clp::CommandLine;
using elrat::clp::Parser;
using elrat::clp::ParserWrapper;
using elrat::clp::NativeParser;

//
// Implementation of private header "inc/parser.hpp"
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
// ---------------------------------------------------------------------------
//
//

THandler::THandler()
: mState{std::make_unique<InitialState>(*this,mCmdLine)}
{
}

void THandler::handle(const std::string& token)
{
  mState->handle(token);
}

CommandLine&& THandler::fetch() 
{
  return std::move(mCmdLine);
}

THandler::State::State(THandler& tokenHandler, CommandLine& commandLine)
: token_handler{tokenHandler}
, command_line{commandLine}
{
}

THandler::State::~State() 
{
}

void THandler::InitialState::handle(const std::string& token)
{
  if ( !IsIdentifierPlus(token) ) {
    throw_invalid_argument(token);
  }
  command_line.setCommand(token);
  token_handler.setNextState<DefaultState>();
}

void THandler::DefaultState::handle(const std::string& token)
{
  if ( IsEqualSign(token) )
  {
    throw_invalid_argument(token + " not allowed in this context.");
  }
  if ( IsOptionPack(token) ) 
  {
    bool redundantOptionsFound{ !add_option_pack(&command_line, token) };
    if (redundantOptionsFound)
    {
      throw_invalid_argument(token + " contains option that already exists.");
    }
  }
  else if ( IsOption(token) ) {
    if ( !add_long_option(&command_line, token) )
      throw_invalid_argument(token + " already exists." );
    token_handler.setNextState<ReceivedOptionState>();
  }
  else {
    command_line.addCommandParameter(token);
  }
}

void THandler::ReceivedOptionState::handle(const std::string& token)
{
  if( IsEqualSign(token) ) 
  {
    token_handler.setNextState<ReceivedEqualSignState>();
  }
  else {
    DefaultState::handle(token);
  }
}

void THandler::ReceivedEqualSignState::handle(const std::string& token)
{
  command_line.addOptionParameter(token);
  token_handler.setNextState<DefaultState>();
}

//
// Implementation of public header "elrat/clp.hpp"
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

    THandler token_handler;
    for(auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        token_handler.handle(*it);
    }
    return token_handler.fetch();
}

const std::string& NativeParser::getSyntaxDescription() const 
{
    return SyntaxDescription;
}

