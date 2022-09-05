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
    void append(const std::string&);
    virtual ~Exception();
protected:
    Exception(
        const std::string& category,
        const std::string& subcategory,
        const std::string& argument);
private:
    std::string message;
};

class InitializationException
: public Exception
{
public: 
    InitializationException(
        const std::string& subcategory,
        const std::string& argument );    
};

class InputException
: public Exception
{
public:
    InputException(
        const std::string& subcategory,
        const std::string& argument );
};

} // namespace clp
} // namespace elrat

#endif

