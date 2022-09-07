#include "elrat/clp/errorhandling.hpp" // public header
#include "errorhandling.hpp" // private

#include <sstream>

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

void clp::Exception::append(const std::string& argument)
{
    message += argument;
}

const char* clp::Exception::what() const noexcept
{
    return message.data();
}

clp::InitializationException::InitializationException(
    const std::string& subcategory,
    const std::string& argument )
: Exception("InitializationException",subcategory,argument)
{
}

clp::InputException::InputException(
    const std::string& subcategory,
    const std::string& argument )
: Exception("InputException",subcategory,argument)
{
}

void ThrowException::EmptyStringAssignment(const std::string& argument)
{
    throw clp::InitializationException("Empty string assignment", argument);
}

void ThrowException::NullPointerAssignment(const std::string& argument)
{
    throw clp::InitializationException("Null pointer assignment", argument);
}

void ThrowException::InvalidParameterConfiguration(const std::string& argument)
{
    throw clp::InitializationException("Invalid parameter configuration", argument);
}

void ThrowException::AlreadyInUse(const std::string& argument)
{
    throw clp::InitializationException("Already in use", argument );
}

void ThrowException::CommandNotFound(const std::string& argument)
{
    throw clp::InitializationException("Command not found", argument );
}


void ThrowException::InvalidParameterType(const std::string& argument)
{
    throw clp::InputException("Invalid parameter type", argument);
}

void ThrowException::InvalidParameterValue(const std::string& argument)
{
    throw clp::InputException("Invalid parameter value", argument);
}

void ThrowException::MissingParameters(int missing)
{
    std::string msg;
    if (missing)
    {
        msg = std::string("At least ") 
            + std::to_string(missing) 
            + " parameters are missing";
    }
    throw clp::InputException("Missing parameter. ", msg);
}

void ThrowException::TooManyParameters(int actual, int expected)
{
    std::string msg{};
    if ( actual > expected )
    {
	    std::stringstream ss;
	    ss << "Expected at most "
	        << expected
	        << ", but found "
	        << actual;
	    std::getline(ss,msg);
    }
    throw clp::InputException("Too many parameters.", msg);
}

void ThrowException::UnrecognizedOption(const std::string& argument)
{
    throw clp::InputException("Unrecognized option", argument);
}

void ThrowException::UnrecognizedCommand(const std::string& argument)
{
    throw clp::InputException("Unrecognized command", argument);
}

