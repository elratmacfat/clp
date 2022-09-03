#include "elrat/clp/errorhandling.hpp" // public header
#include "errorhandling.hpp" // private

elrat::clp::Exception::Exception(const std::string& prefix, const std::string& msg)
: message{prefix}
{
    if (msg.size())
    {
        message += ": [";
        message += msg;
        message += "]";
    }
}

const char* elrat::clp::Exception::what() const noexcept
{
    return message.data();
}

void throwNullPointerAssignmentException(const std::string& msg)
{
    throw elrat::clp::Exception("Nullpointer assignment", msg);
}

void throwParameterConfigurationException(const std::string& msg)
{
    throw elrat::clp::Exception("Invalid parameter configuration", msg);
}

void throwAlreadyInUseException(const std::string& msg)
{
    throw elrat::clp::Exception("Name already in use", msg);
}

