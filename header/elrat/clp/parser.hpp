#ifndef ELRAT_CLP_PARSER_HPP
#define ELRAT_CLP_PARSER_HPP

#include <string>

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
   
} // namespace clp
} // namespace elrat
#endif

