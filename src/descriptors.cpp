// Project......: Command Line Processor (clp)
// File.........: src/descriptors.cpp
// Author.......: elratmacfat
// Description..: 
//
#include <elrat/clp/descriptors.hpp>
#include "errorhandling.hpp"
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

bool clp::isMatch( const ValidationResult& v )
{
    return (v == ValidationResult::Match);
}

bool clp::isNoMatch( const ValidationResult& v )
{
    return (v == ValidationResult::NoMatch );
}

bool clp::isInvalid( const ValidationResult& v )
{
    return (!isMatch(v) && !isNoMatch(v));
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

HasDescription::HasDescription(const std::string& s)
: description{s}
{
}

const std::string& HasDescription::getDescription() const
{
    return description;
}

void HasParameters::initialize()
{
    // Ensure that optional parameters are not followed by parameters declared mandatory.
    numberOfRequiredParameters = 0;
    bool previousParameterWasOptional{false};
    for( auto& p : parameters )
    {
        if (p->parameterIsRequired())
        {
            numberOfRequiredParameters++;
            if ( previousParameterWasOptional )
                throwParameterConfigurationException();
        }
        else
        {
            previousParameterWasOptional = true;
        }
    }
}

HasParameters::HasParameters(const ParameterDescriptors& params)
: parameters{params}
{
    initialize();
}

HasParameters::HasParameters(ParameterDescriptors&& params)
: parameters{std::move(params)}
{
    initialize();
}

const ParameterDescriptors& HasParameters::getParameters() const 
{
    return parameters;
}

int HasParameters::getRequiredParameterCount() const
{
    return numberOfRequiredParameters;
}

ValidationResult HasParameters::validate(const Arguments& args) const
{
    if ( args.size() > parameters.size() )
        return ValidationResult::TooManyParameters;
    if ( args.size() < numberOfRequiredParameters )
        return ValidationResult::MissingParameters;
    for( int i{0}; i < args.size(); ++i )
    {
        ValidationResult result{ parameters[i]->validate( args[i] ) };
        if ( isInvalid(result) )
            return result;
    }
    return ValidationResult::Match;
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

ValidationResult ParameterDescriptor::validate(const Argument& arg) const
{
    if ( !type_checker(arg) )
        return ValidationResult::InvalidParameterType;
    for( auto& constraint : constraints )
    {
        if ( !constraint->validate(arg) )
            return ValidationResult::InvalidParameterValue;
    }
    return ValidationResult::Match;
}

//-----------------------------------------------------------------------------

OptionDescriptor::OptionDescriptor(
    const std::string& name,
    const std::string& description,
    const ParameterDescriptors& parameter_descriptors )
: HasName(name)
, HasDescription(description)
, HasParameters{parameter_descriptors}
{
}

ValidationResult OptionDescriptor::validate( 
    const Argument& name, 
    const Arguments& args ) const
{
    if ( name != this->getName() )
        return ValidationResult::NoMatch;

    return HasParameters::validate( args );
}

//-----------------------------------------------------------------------------

CommandDescriptor::CommandDescriptor(
    const std::string& name,
    const std::string& description,
    const ParameterDescriptors& parameter_descriptors,
    const OptionDescriptors& option_descriptors )
: HasName(name)
, HasDescription(description)
, HasParameters(parameter_descriptors)
, options{option_descriptors}
{
}

const OptionDescriptors& CommandDescriptor::getOptions() const
{
    return options;
}

ValidationResult CommandDescriptor::validate( const CommandLine& cmdline) const
{
    if ( cmdline.getCommand() != this->getName() )
        return ValidationResult::NoMatch;

    ValidationResult result{ HasParameters::validate(cmdline.getCommandParameters()) };
    if ( result != ValidationResult::Match )
        return result;

    for( int i{0}; i < cmdline.getOptionCount(); ++i )
    {
        auto& opt_name{ cmdline.getOption(i) };
        auto& opt_parameters{ cmdline.getOptionParameters(i) };
        for( auto& option_descriptor : options )
        {
            result = option_descriptor->validate( opt_name, opt_parameters );
            if ( isInvalid(result) )
                return result;
            if ( isMatch(result) )
                break;
        }
        if ( isNoMatch(result) ) 
            return result; 
    }
    return result;
}

//-----------------------------------------------------------------------------

CommandDescriptorsPtr CommandDescriptors::Create(const std::string& name)
{
    return std::shared_ptr<CommandDescriptors>(new CommandDescriptors(name));
}

void CommandDescriptors::attach(CommandDescriptorPtr p)
{
    for( auto descriptor : command_descriptors )
        if ( p == descriptor || p->getName() == descriptor->getName() )
            throwAlreadyInUseException("CommandDescriptors::attach");
    command_descriptors.push_back(p);
}

ValidationResult CommandDescriptors::validate(const CommandLine& cmdline) const 
{
    for( auto descriptor : command_descriptors )
    {
        ValidationResult result = descriptor->validate(cmdline);
        if ( result != ValidationResult::InvalidCommand )
            return result;
    }
    return ValidationResult::InvalidCommand;
}

CommandDescriptors::CommandDescriptors(const std::string& name)
: HasName(name)
{
}
