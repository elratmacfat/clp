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
        const std::string& subcategory = "",
        const std::string& argument = "");    
};

class NullptrAssignmentException
: public InitializationException
{
public:
    NullptrAssignmentException(const std::string& = "");
};

class EmptyStringException 
: public InitializationException
{
public:
    EmptyStringException(const std::string& = "");
};

class InvalidParameterConfigurationException
: public InitializationException
{
public:
    InvalidParameterConfigurationException(const std::string& = "");
};

class AlreadyInUseException
: public InitializationException
{
public:
    AlreadyInUseException(const std::string& = "");
};

class CommandNotFoundException
: public InitializationException
{
public:
    CommandNotFoundException(const std::string& = "");
};

class InputException
: public Exception
{
public:
    InputException(
        const std::string& subcategory = "",
        const std::string& argument = "" );
};

class InvalidParameterTypeException
: public InputException
{
public:
    InvalidParameterTypeException(const std::string&);
};

class InvalidParameterValueException
: public InputException
{
public:
    InvalidParameterValueException(const std::string&);
};

class MissingParametersException
: public InputException
{
public:
    MissingParametersException(int);
};

class TooManyParametersException
: public InputException
{
public:
    TooManyParametersException(int actual, int expected);
};

class InvalidOptionException
: public InputException
{
public:
    InvalidOptionException(const std::string&);
};

} // namespace clp
} // namespace elrat

#endif

