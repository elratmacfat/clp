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

class THandler
{
public:
    THandler()=delete;
    THandler( elrat::clp::CommandLine& destination );
    THandler(const THandler&)=delete;
    THandler(THandler&&)=default;
    THandler& operator=(const THandler&)=delete;
    THandler& operator=(THandler&&)=default;
    ~THandler()=default;
    void handle(const std::string& token);
    
    template <class NextState>
    void setNextState() {
        state = std::make_unique<NextState>(*this, destination);
    }
private:
    class State;
    class InitialState;        
    class DefaultState;        
    class ReceivedOptionState;  
    class ReceivedEqualSignState;

    elrat::clp::CommandLine&    destination;
    std::unique_ptr<State>      state;
};

class THandler::State 
{
public:
    State(THandler& parent, elrat::clp::CommandLine& destination);
    virtual ~State();
    virtual void handle(const std::string& token) = 0;
protected:
    THandler& parent;
    elrat::clp::CommandLine& destination;
};

class THandler::InitialState : public THandler::State 
{
public:
    using State::State;
    void handle(const std::string& token);
};

class THandler::DefaultState : public THandler::State 
{
public:
    using State::State;
    void handle(const std::string& token);
};

class THandler::ReceivedOptionState: public THandler::DefaultState 
{
public:
    using DefaultState::DefaultState;
    void handle(const std::string& token);
};

class THandler::ReceivedEqualSignState: public THandler::State 
{
public:
    using State::State;
    void handle(const std::string& token);
};


class TokenHandler
{
public:
    TokenHandler(elrat::clp::CommandLine*);
    virtual ~TokenHandler();
    virtual State handle( const std::string& token ) = 0;
protected:
    elrat::clp::CommandLine* target;
};

class TokenHandlerExpectingCommand
: public TokenHandler
{
public:
    using TokenHandler::TokenHandler;
    virtual State handle(const std::string& token);
};

class TokenHandlerForCommandParameter
: public TokenHandler
{
public:
    using TokenHandler::TokenHandler;
    virtual State handle(const std::string& token);
};

class TokenHandlerForAnythingElse
: public TokenHandlerForCommandParameter
{
public:
    using TokenHandlerForCommandParameter::TokenHandlerForCommandParameter;
    virtual State handle(const std::string& token);
};

class TokenHandlerForOptionParameter
: public TokenHandler
{
public:
    using TokenHandler::TokenHandler;
    virtual State handle(const std::string& token);
};

class TokenHandlerOnOptionReception
: public TokenHandlerForAnythingElse
{
public:
    using TokenHandlerForAnythingElse::TokenHandlerForAnythingElse;
    virtual State handle(const std::string& token);
};

class TokenHandlerFactory
{
public:
    TokenHandlerFactory(elrat::clp::CommandLine*);
    std::unique_ptr<TokenHandler> createTokenHandler( State );
private:
    elrat::clp::CommandLine* target;
};

#endif
