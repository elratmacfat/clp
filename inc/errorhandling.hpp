#ifndef ERRORHANDLING_HPP
#define ERRORHANDLING_HPP

#include <string>

void throwNullPointerAssignmentException(const std::string& = "");
void throwParameterConfigurationException(const std::string& = "");
void throwAlreadyInUseException(const std::string& = "");

#endif

