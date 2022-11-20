#ifndef ELRAT_CLP_PARSERWRAPPER_HPP
#define ELRAT_CLP_PARSERWRAPPER_HPP 

#include <functional>
#include <string>

#include <elrat/clp/parser.hpp>

namespace elrat {
namespace clp {

class ParserWrapper : public Parser
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


} // clp
} // elrat

#endif
