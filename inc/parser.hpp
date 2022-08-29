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

#include <elrat/clp.hpp>

#include "regex.hpp"

std::vector<std::string> tokenize(const std::string& input);

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
    std::unique_ptr<TokenHandler> create( State );
private:
    elrat::clp::CommandLine* target;
};

extern const RegEx IsIdentifierPlus;
extern const RegEx IsOption;
extern const RegEx IsOptionPack;
extern const RegEx IsEqualSign;

void add_option(elrat::clp::CommandLine*, const std::string& token);
void add_option_pack(elrat::clp::CommandLine*, const std::string& token);


#endif
