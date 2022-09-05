#ifndef ERRORHANDLING_HPP
#define ERRORHANDLING_HPP

#include <string>

class ThrowException 
{
public:
    // InitializationException
    static void NullPointerAssignment(const std::string& = "");
    static void InvalidParameterConfiguration(const std::string& = "");
    static void NameAlreadyInUse(const std::string& = "");
    // InputException
    static void InvalidParameterType(const std::string& = "");
    static void InvalidParameterValue(const std::string& = "");
    static void MissingParameters(int missing = 0);
    static void TooManyParameters(int actual = 0,int expected = 0);
    static void UnrecognizedOption(const std::string& = "");
    static void UnrecognizedCommand(const std::string& = "");
};

#endif

