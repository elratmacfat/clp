#ifndef DESCRIPTORS_T_INPUT_HPP
#define DESCRIPTORS_T_INPUT_HPP

#include <string>
#include <vector>

#include <elrat/clp/descriptors.hpp>
namespace ParameterValidation 
{
    extern const std::vector<std::string> hexadecimal_zero;
    extern const std::vector<std::string> hexadecimals;
    extern const std::vector<std::string> positive_zero;
    extern const std::vector<std::string> negative_zero;
    extern const std::vector<std::string> positive_integers;
    extern const std::vector<std::string> negative_integers;
    extern const std::vector<std::string> floating_point_numbers;
    extern const std::vector<std::string> identifiers;
    extern const std::vector<std::string> names;
    extern const std::vector<std::string> win_paths;
    extern const std::vector<std::string> nix_paths;
    extern const std::vector<std::string> email_addresses;
    extern const std::vector<std::string> invalid_email_addresses;
    
    // ParameterValidation/Constraint
    extern const std::vector<std::string> range_0_to_9; 
    extern const std::vector<std::string> range_10_to_19; 
    extern const std::vector<std::string> range_20_to_29; 
}

namespace OptionValidation 
{
    elrat::clp::OptionDescriptorPtr createOptionDescriptor();
    extern const std::vector<std::vector<std::string>> valid_parameters;
    extern const std::vector<std::vector<std::string>> missing_parameters;
    extern const std::vector<std::vector<std::string>> too_many_parameters;
    extern const std::vector<std::vector<std::string>> invalid_parameter_types;
    extern const std::vector<std::vector<std::string>> invalid_parameter_values;
}

namespace CommandValidation
{
    elrat::clp::CommandDescriptorPtr createCommandDescriptor();

    std::vector<elrat::clp::CommandLine> createValid();
    std::vector<elrat::clp::CommandLine> createUnrecognized();
    std::vector<elrat::clp::CommandLine> createMissingParameters();
    std::vector<elrat::clp::CommandLine> createTooManyParameters();
}

#endif

