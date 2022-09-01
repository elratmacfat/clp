// Project......: Command Line Processor (clp)
// File.........: inc/parser.hpp
// Author.......: elratmacfat
// Description..: 
//

#ifndef PARSER_HPP
#define PARSER_HPP

#include <exception>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include <elrat/clp.hpp>

#include "regex.hpp"

class ModifiableCommandLine
: public elrat::clp::CommandLine
{
public:
    void setCommand(const std::string&);
    void addParameter(const std::string&);
    void addOption(const std::string&);
    void addOptionParameter(const std::string&);
};

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

bool add_option(ModifiableCommandLine*, const std::string& option);
bool add_long_option(ModifiableCommandLine*, const std::string& token);
bool add_option_pack(ModifiableCommandLine*, const std::string& token);

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
    TokenHandler(ModifiableCommandLine*);
    virtual ~TokenHandler();
    virtual State handle( const std::string& token ) = 0;
protected:
    ModifiableCommandLine* target;
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
    TokenHandlerFactory(ModifiableCommandLine*);
    std::unique_ptr<TokenHandler> createTokenHandler( State );
private:
    ModifiableCommandLine* target;
};

#endif
