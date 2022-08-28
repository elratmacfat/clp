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


class TokenHandler
{
public:
    enum class State 
    {
       Received_Invalid_Token = 0,
       Expecting_Command,
       Expecting_CommandParameter_Option,
       Expecting_OptionParameter,
       Expecting_CommandParameter_Option_EqualSign
    };
    TokenHandler(elrat::clp::CommandLine*);
    virtual ~TokenHandler();
    virtual State handle( const std::string& token ) = 0;
protected:
    elrat::clp::CommandLine* target;
};

class TH_Command
: public TokenHandler
{
public:
    using TokenHandler::TokenHandler;
    virtual TokenHandler::State handle(const std::string& token);
};

class TH_CommandParameter
: public TokenHandler
{
public:
    using TokenHandler::TokenHandler;
    virtual TokenHandler::State handle(const std::string& token);
};

class TH_CommandParameter_Option
: public TH_CommandParameter
{
public:
    using TH_CommandParameter::TH_CommandParameter;
    virtual TokenHandler::State handle(const std::string& token);
};

class TH_OptionParameter
: public TokenHandler
{
public:
    using TokenHandler::TokenHandler;
    virtual TokenHandler::State handle(const std::string& token);
};

class TH_CommandParameter_Option_EqualSign
: public TH_CommandParameter_Option
{
public:
    using TH_CommandParameter_Option::TH_CommandParameter_Option;
    virtual TokenHandler::State handle(const std::string& token);
};

class TokenHandlerFactory
{
public:
    TokenHandlerFactory(elrat::clp::CommandLine*);
    std::unique_ptr<TokenHandler> create( TokenHandler::State );
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
