#include "elrat/clp/errorhandling.hpp"

using namespace elrat;

clp::Exception::Exception(
    const std::string& category, 
    const std::string& subcategory,
    const std::string& argument)
: message{category}
{
    message = category + ": " + subcategory + " [" + argument + "]";   
}

clp::Exception::~Exception()
{
}

void clp::Exception::append(const std::string& arg)
{
    message += arg;
}

const char* clp::Exception::what() const noexcept
{
    return message.data();
}

clp::InitializationException::InitializationException(
    const std::string& subcategory,
    const std::string& arg )
: Exception("InitializationException",subcategory,arg)
{
}

clp::NullptrAssignmentException::NullptrAssignmentException(
    const std::string& arg)
: InitializationException("Nullptr Assignment",arg)
{
}

clp::EmptyStringException::EmptyStringException(
    const std::string& arg)
: InitializationException("Empty String",arg)
{
}

clp::InvalidParameterConfigurationException::InvalidParameterConfigurationException(
    const std::string& arg)
: InitializationException("Invalid parameter configuration", arg)
{
}

clp::AlreadyInUseException::AlreadyInUseException(
    const std::string& arg)
: InitializationException("Already in use", arg)
{
}

clp::CommandNotFoundException::CommandNotFoundException(
    const std::string& arg)
: InitializationException("Already in use", arg)
{
}


clp::InputException::InputException(
    const std::string& subcategory,
    const std::string& arg )
: Exception("InputException",subcategory,arg)
{
}

clp::InvalidParameterTypeException::InvalidParameterTypeException(
    const std::string& arg)
: InputException("Invalid Parameter Type", arg)
{
}

clp::InvalidParameterValueException::InvalidParameterValueException(
    const std::string& arg)
: InputException("Invalid Parameter Value", arg)
{
}

clp::MissingParametersException::MissingParametersException(
    int number)
: InputException("Missing Parameters", std::to_string(number))
{
}

clp::TooManyParametersException::TooManyParametersException(
    int actual, int expected)
: InputException(
    "Too Many Parameters", 
    std::to_string(actual) 
    + "/" 
    + std::to_string(expected))
{
}

clp::InvalidOptionException::InvalidOptionException(const std::string& arg)
: InputException("Invalid option", arg)
{
}

