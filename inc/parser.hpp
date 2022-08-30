// Project......: Command Line Processor (clp)
// File.........: inc/parser.hpp
// Author.......: elratmacfat
// Description..: 
//

#ifndef PARSER_HPP
#define PARSER_HPP

#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "modifiablecommandline.hpp"
#include <elrat/clp.hpp>

#include "regex.hpp"

extern const RegEx IsIdentifierPlus;
extern const RegEx IsOption;
extern const RegEx IsOptionPack;
extern const RegEx IsEqualSign;

std::vector<std::string> tokenize(const std::string& input);
bool option_exists(ModifiableCommandLine*, const std::string& option);
void add_option(ModifiableCommandLine*, const std::string& option);
void add_long_option(ModifiableCommandLine*, const std::string& token);
void add_option_pack(ModifiableCommandLine*, const std::string& token);
void throw_invalid_argument(const char*);
void throw_invalid_token();
void throw_invalid_state();

enum class State
{
    Expecting_Command,
    Received_Invalid_Token,
    Received_Option,        
    Received_EqualSign,    
    Received_Anything_Else
};

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
    std::unique_ptr<TokenHandler> create( State );
private:
    ModifiableCommandLine* target;
};

#endif
