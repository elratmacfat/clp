#ifndef ELRAT_CLP_NATIVEPARSER_HPP
#define ELRAT_CLP_NATIVEPARSER_HPP 

#include <elrat/clp/parser.hpp>

namespace elrat {
namespace clp {

class NativeParser : public Parser
{
public:
    virtual CommandLine parse( const std::string& ) const;
    virtual const std::string& getSyntaxDescription() const;
private:
    static const std::string SyntaxDescription;
};


} // clp 
} // elrat 

#endif
