#ifndef DESCRIPTORS_T_UTIL_HPP
#define DESCRIPTORS_T_UTIL_HPP

#include <elrat/clp.hpp>

#include <string>
#include <vector>

template <class T> 
std::vector<std::string> convertRangeToStrings(
    T from, 
    T to, 
    T increment = static_cast<T>(1) );

void Check(
    elrat::clp::TypeChecker, 
    const std::vector<std::string>&);
void Check(
    elrat::clp::ConstraintPtr, 
    const std::vector<std::string>&);
void Check( 
    elrat::clp::OptionDescriptorPtr, 
    const std::vector<std::string>&);
void Check( 
    elrat::clp::CommandDescriptorPtr cmd_desc, 
    const elrat::clp::CommandLine& input,
    elrat::clp::ValidationResult expected );

void FailCheck(
    elrat::clp::ConstraintPtr, 
    const std::vector<std::string>&);
void FailCheck(
    elrat::clp::TypeChecker, 
    const std::vector<std::string>&);
void FailCheck(
    elrat::clp::OptionDescriptorPtr, 
    const std::vector<std::string>&);

std::string toString(
    elrat::clp::ValidationResult);

std::string toString(
    const std::vector<std::string>&); 

// Template implementation
// vvvvvvvvvvvvvvvvvvvvvvv

template <class T>
std::vector<std::string> convertRangeToStrings(
    T from, 
    T to, 
    T increment )
{
    std::vector<std::string> result{};
    result.reserve( (to-from)/increment + 1 );
    while ( from <= to )
    {
        result.push_back(std::to_string(from));
        from+=increment;
    }
    return result;
}

#endif

