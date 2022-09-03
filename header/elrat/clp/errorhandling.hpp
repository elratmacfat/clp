#ifndef ELRAT_CLP_ERRORHANDLING_HPP
#define ELRAT_CLP_ERRORHANDLING_HPP

#include <stdexcept>
#include <string>

namespace elrat {
namespace clp {

class Exception
: public std::exception
{
public: 
    virtual const char* what() const noexcept;
    Exception(const std::string&, const std::string&);
private:
    std::string message;
};


} // namespace clp
} // namespace elrat

#endif

