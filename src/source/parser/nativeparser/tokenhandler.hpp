#ifndef NATIVEPARSER_TOKENHANDLER_HPP
#define NATIVEPARSER_TOKENHANDLER_HPP

#include <memory>
#include <string>

#include "elrat/clp/commandline.hpp"

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
    bool add_option(const std::string& option);
    bool add_long_option(const std::string& token);
    bool add_option_pack(const std::string& token);

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

