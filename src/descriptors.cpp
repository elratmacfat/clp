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
    return (IsWindowsPath(s) || IsUnixPath(s));
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
                ThrowException::InvalidParameterConfiguration(p->getName());
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

void HasParameters::validate(const Arguments& args) const
{
    if ( args.size() > parameters.size() )
        ThrowException::TooManyParameters( args.size(), parameters.size() );
    if ( args.size() < numberOfRequiredParameters )
        ThrowException::MissingParameters(numberOfRequiredParameters - args.size());
    for( int i{0}; i < args.size(); ++i )
        parameters[i]->validate( args[i] );
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

void ParameterDescriptor::validate(const Argument& arg) const
{
    if ( !type_checker(arg) )
        ThrowException::InvalidParameterType(arg);
    for( auto& constraint : constraints )
    {
        if ( !constraint->validate(arg) )
            ThrowException::InvalidParameterValue(arg);
    }
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

bool OptionDescriptor::validate( const Argument& name,const Arguments& args ) const
{
    if ( name != this->getName() )
    {
        return false;
    } 
    HasParameters::validate( args );
    return true;
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

bool CommandDescriptor::validate( const CommandLine& cmdline) const
{
    if ( cmdline.getCommand() != this->getName() )
        return false;

    HasParameters::validate(cmdline.getCommandParameters());

    for( int i{0}; i < cmdline.getOptionCount(); ++i )
    {
        auto& opt_name{ cmdline.getOption(i) };
        auto& opt_parameters{ cmdline.getOptionParameters(i) };
        bool match;
        for( auto& option_descriptor : options )
        {
            match = option_descriptor->validate(opt_name,opt_parameters);
            if (match)
            {
                break;
            }
        }
        if (!match) 
        {
            ThrowException::UnrecognizedOption(opt_name);
        }
    }
    return true;
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
            ThrowException::NameAlreadyInUse(
                p->getName() + " (CommandDescriptors::attach)");
    command_descriptors.push_back(p);
}

bool CommandDescriptors::validate(const CommandLine& cmdline) const 
{
    for( auto descriptor : command_descriptors )
    {
        bool result{  };
        if ( descriptor->validate(cmdline) )
            return true;
    }
    return false;
}

CommandDescriptors::CommandDescriptors(const std::string& name)
: HasName(name)
{
}
