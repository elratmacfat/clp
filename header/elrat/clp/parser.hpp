#ifndef ELRAT_CLP_PARSER_HPP
#define ELRAT_CLP_PARSER_HPP

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <elrat/clp/commandline.hpp>

namespace elrat {
namespace clp {

class Parser
{
public:
    virtual ~Parser();
    virtual CommandLine parse( const std::string& ) const = 0;
    virtual const std::string& getSyntaxDescription() const = 0;
};
   
class ParserWrapper
: public Parser
{
public:
    using ParsingFunction = std::function<CommandLine(const std::string&)>;
    ParserWrapper( ParsingFunction, const std::string& description );
    virtual CommandLine parse(const std::string&) const;
    virtual const std::string& getSyntaxDescription() const;
private:
    ParsingFunction parsing_function;
    std::string syntax_description;
};

class NativeParser
: public Parser
{
public:
    virtual CommandLine parse( const std::string& ) const;
    virtual const std::string& getSyntaxDescription() const;
private:
    static const std::string SyntaxDescription;
};

} // namespace clp
} // namespace elrat
#endif

