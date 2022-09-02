// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/desc.hpp
// Author.......: elratmacfat
// Description..: 
//
#include <elrat/clp/descriptors.hpp>
#include "regex.hpp"

using namespace elrat;
using namespace elrat::clp;

const bool clp::Mandatory{true};
const bool clp::Optional{false};

bool ParameterType::Any(const std::string& s)
{
    return (s.size() > 0);
}

bool ParameterType::NaturalNumber(const std::string& s)
{
    return IsPositiveDecimal(s) || IsHexaDecimal(s);
}

bool ParameterType::WholeNumber(const std::string& s)
{   
    return IsDecimal(s) || IsHexaDecimal(s);
}

bool ParameterType::RealNumber(const std::string& s)
{
    return IsFloatingPoint(s);
}

bool ParameterType::Name(const std::string& s)
{
    return IsName(s);
}

bool ParameterType::Identifier(const std::string& s)
{
    return IsIdentifier(s);
}

bool ParameterType::Path(const std::string& s)
{
    return IsPath(s);
}


CommandDescriptorPtr clp::makeCommandDescriptor(
    const std::string& name,
    const std::string& description,
    const ParameterDescriptors& parameters,
    const OptionDescriptors& options)
{
    return std::make_shared<CommandDescriptor>(
        name,
        description,
        parameters,
        options);
}

OptionDescriptorPtr clp::makeOptionDescriptor(
    const std::string& name,
    const std::string& description,
    const ParameterDescriptors& parameters )
{
    return std::make_shared<OptionDescriptor>(
        name,
        description,
        parameters);
}

ParameterDescriptorPtr clp::makeParameterDescriptor(
    const std::string& name,
    const std::string& description,
    bool required,
    TypeChecker type_checker,
    Constraints constraints )
{
    return std::make_shared<ParameterDescriptor>(
        name,
        description,
        required,
        type_checker,
        constraints );
}

//-----------------------------------------------------------------------------

HasName::HasName( const std::string& s ) 
: name{s}
{
}

const std::string& HasName::getName() const 
{
    return name;
}

//-----------------------------------------------------------------------------

HasDescription::HasDescription(const std::string& s)
: description{s}
{
}

const std::string& HasDescription::getDescription() const
{
    return description;
}

//-----------------------------------------------------------------------------

ParameterDescriptor::ParameterDescriptor(
    const std::string& name,
    const std::string& description,
    bool isRequired,
    TypeChecker typeChecker,
    Constraints parameter_constraints )
: HasName(name)
, HasDescription(description)
, required{isRequired}
, type_checker{typeChecker}
, constraints{parameter_constraints}
{

}

bool ParameterDescriptor::parameterIsRequired() const
{
    return required;
}

TypeChecker ParameterDescriptor::getTypeChecker() const
{
    return type_checker;
}

const Constraints& ParameterDescriptor::getConstraints() const
{
    return constraints;
}

ValidationResult ParameterDescriptor::validate(const std::string& p) 
{
    if (!type_checker(p))
        return ValidationResult::InvalidParameter;
    for( auto& c : constraints )
    {
        if (!c->validate(p))
        {
            return ValidationResult::InvalidParameter;
        }
    }
    return ValidationResult::Valid;
}

//-----------------------------------------------------------------------------

OptionDescriptor::OptionDescriptor(
    const std::string& name,
    const std::string& description,
    const ParameterDescriptors& parameter_descriptors )
: HasName(name)
, HasDescription(description)
, parameters{parameter_descriptors}
{
    // Ensure that optional parameters are not followed by parameters declared mandatory.
    numberOfRequiredParameters = 0;
    bool previousParameterWasOptional{false};
    for( auto& p : parameter_descriptors )
    {
        if (p->parameterIsRequired())
        {
            numberOfRequiredParameters++;
            if ( previousParameterWasOptional )
                throw std::invalid_argument(
                    "Optional parameters cannot be followed by one that "
                    "is required. Revisit your command descriptor "
                    "initialization.");
        }
        else
        {
            previousParameterWasOptional = true;
        }
    }
}

const ParameterDescriptors& OptionDescriptor::getParameters() const
{
    return parameters;
}

ValidationResult OptionDescriptor::validate( const std::vector<std::string>& arguments )
{
    if ( arguments.size() < numberOfRequiredParameters )
        return ValidationResult::MissingParameter;
    if ( arguments.size() > parameters.size() )
        return ValidationResult::InvalidParameter;

    for( int i{0}; i < arguments.size(); i++ )
    {
        ValidationResult result {parameters[i]->validate(arguments[i])};
        if ( result != ValidationResult::Valid )
            return result;
    }
    return ValidationResult::Valid;
}

//-----------------------------------------------------------------------------

CommandDescriptor::CommandDescriptor(
    const std::string& name,
    const std::string& description,
    const ParameterDescriptors& command_parameters,
    const OptionDescriptors& option_descriptors )
: OptionDescriptor(name,description,command_parameters)
, options{option_descriptors}
{
}

const OptionDescriptors& CommandDescriptor::getOptions() const
{
    return options;
}

ValidationResult CommandDescriptor::validate( const CommandLine& command_line )
{
    if (!command_line)
        return ValidationResult::InvalidCommand;
    
    // Reminder: OptionDesriptor is the CommandDescriptor's base class!
    auto& command_parameters{ command_line.getCommandParameters() };
    ValidationResult result{ OptionDescriptor::validate(command_parameters) };
    if ( result != ValidationResult::Valid ) 
    {
        if ( result == ValidationResult::InvalidOption ) 
            result = ValidationResult::InvalidCommand;
        return result;
    }

    for( int i{0}; i < command_line.getOptionCount(); i++ )
    {
        auto& option_parameters{ command_line.getOptionParameters(i) };
        for( auto option_descriptor : options )
        {
            result = option_descriptor->validate( option_parameters );
            
            if ( result == ValidationResult::Valid )
                break;

            if ( result != ValidationResult::InvalidOption )
                return result;
        }
    }
    return result;
}


