
#include "elrat/clp/errorhandling.hpp"

#include "tokenhandler.hpp"
#include "regex.hpp"

using elrat::clp::CommandLine;
using elrat::clp::InputException;

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

TokenHandler::TokenHandler()
: mState{std::make_unique<InitialState>(*this,mCmdLine)}
{
}

void TokenHandler::handle(const std::string& token)
{
  mState->handle(token);
}

CommandLine&& TokenHandler::fetch() 
{
  return std::move(mCmdLine);
}

TokenHandler::State::State(TokenHandler& tokenHandler, CommandLine& commandLine)
: token_handler{tokenHandler}
, command_line{commandLine}
{
}

TokenHandler::State::~State() 
{
}

bool TokenHandler::State::add_option(const std::string& option)
{
    if (command_line.optionExists(option))
        return false;
    command_line.addOption(option);
    return true;
}

bool TokenHandler::State::add_long_option(const std::string& token)
{
    static const int preceeding_dashes = 2;
    auto option{ token.substr(preceeding_dashes) };
    return add_option(option);
}

bool TokenHandler::State::add_option_pack(const std::string& token)
{
    for( int i{1}; i < token.size(); i++ ) // omit the single preceeding dash
    {
        const std::string option(1,token[i]);
        if (!add_option(option))
            return false;
    }
    return true;
}


void TokenHandler::InitialState::handle(const std::string& token)
{
  if ( !IsIdentifierPlus(token) ) {
    throw InputException(token," is not an Identifier-Plus");
  }
  command_line.setCommand(token);
  token_handler.setNextState<DefaultState>();
}

void TokenHandler::DefaultState::handle(const std::string& token)
{
  if ( IsEqualSign(token) )
  {
    throw InputException(token, " not allowed in this context.");
  }
  if ( IsOptionPack(token) ) 
  {
    bool redundantOptionsFound{ !add_option_pack(token) };
    if (redundantOptionsFound)
    {
      throw InputException(token, "contains option that already exists.");
    }
  }
  else if ( IsOption(token) ) {
    if ( !add_long_option(token) )
      throw InputException(token, "Already exists");
    token_handler.setNextState<ReceivedOptionState>();
  }
  else {
    command_line.addCommandParameter(token);
  }
}

void TokenHandler::ReceivedOptionState::handle(const std::string& token)
{
  if( IsEqualSign(token) ) 
  {
    token_handler.setNextState<ReceivedEqualSignState>();
  }
  else {
    DefaultState::handle(token);
  }
}

void TokenHandler::ReceivedEqualSignState::handle(const std::string& token)
{
  command_line.addOptionParameter(token);
  token_handler.setNextState<DefaultState>();
}
