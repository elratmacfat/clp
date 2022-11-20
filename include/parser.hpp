#ifndef PARSER_HPP
#define PARSER_HPP

#include <exception>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include <elrat/clp.hpp>

#include "regex.hpp"

enum class State
{
    Expecting_Command,
    Received_Invalid_Token,
    Received_Option_Twice,
    Received_Option,        
    Received_EqualSign,    
    Received_Anything_Else
};

std::string to_string(State);
bool is_error(State);

std::vector<std::string> tokenize(const std::string& input);

bool add_option(elrat::clp::CommandLine*, const std::string& option);
bool add_long_option(elrat::clp::CommandLine*, const std::string& token);
bool add_option_pack(elrat::clp::CommandLine*, const std::string& token);

void throw_runtime_error(const std::string&);
void throw_invalid_argument(const std::string&);
void throw_invalid_argument(const State& state, const std::string& token);

extern const RegEx IsIdentifierPlus;
extern const RegEx IsOption;
extern const RegEx IsOptionPack;
extern const RegEx IsEqualSign;

class TokenHandler
{
public:
    TokenHandler();
    TokenHandler(const TokenHandler&)=delete;
    TokenHandler(TokenHandler&&)=default;
    TokenHandler& operator=(const TokenHandler&)=delete;
    TokenHandler& operator=(TokenHandler&&)=default;
    ~TokenHandler()=default;
    void handle(const std::string& token);
    
    template <class NextState>
    void setNextState() {
        mState = std::make_unique<NextState>(*this,mCmdLine);
    }

    elrat::clp::CommandLine&& fetch();
private:
    class State;
    class InitialState;        
    class DefaultState;        
    class ReceivedOptionState;  
    class ReceivedEqualSignState;

    elrat::clp::CommandLine mCmdLine;
    std::unique_ptr<State>  mState;
};

class TokenHandler::State 
{
public:
    State(TokenHandler&, elrat::clp::CommandLine&);
    virtual ~State();
    virtual void handle(const std::string& token) = 0;
protected:
    TokenHandler&                   token_handler;
    elrat::clp::CommandLine&    command_line;
};

class TokenHandler::InitialState : public TokenHandler::State 
{
public:
    using State::State;
    void handle(const std::string& token);
};

class TokenHandler::DefaultState : public TokenHandler::State 
{
public:
    using State::State;
    void handle(const std::string& token);
};

class TokenHandler::ReceivedOptionState: public TokenHandler::DefaultState 
{
public:
    using DefaultState::DefaultState;
    void handle(const std::string& token);
};

class TokenHandler::ReceivedEqualSignState: public TokenHandler::State 
{
public:
    using State::State;
    void handle(const std::string& token);
};

#endif
