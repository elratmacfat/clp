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
    
    // ParameterValidation/Constraint
    extern const std::vector<std::string> range_0_to_9; 
    extern const std::vector<std::string> range_10_to_19; 
    extern const std::vector<std::string> range_20_to_29; 
}

namespace OptionValidation 
{
    elrat::clp::OptionDescriptorPtr createOptionDescriptor();
    extern const std::vector<std::vector<std::string>> valid_parameters;
    extern const std::vector<std::vector<std::string>> invalid_parameters;
}

namespace CommandValidation
{
	elrat::clp::CommandDescriptorPtr createCommandAddPlatform();
	elrat::clp::CommandDescriptorPtr createCommandSetTemperature();
	
	elrat::clp::CommandDescriptorsPtr createCommandDescriptors();
	
	std::vector<elrat::clp::CommandLine> createValidCommandLines();
	std::vector<elrat::clp::CommandLine> createInvalidCommandLines();
}

#endif

