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

bool clp::Any(const std::string& s)
{
    return (s.size() > 0);
}

bool clp::NaturalNumber(const std::string& s)
{
    return IsPositiveDecimal(s) || IsHexaDecimal(s);
}

bool clp::WholeNumber(const std::string& s)
{   
    return IsDecimal(s) || IsHexaDecimal(s);
}

bool clp::RealNumber(const std::string& s)
{
    return IsFloatingPoint(s);
}

bool clp::Name(const std::string& s)
{
    return IsName(s);
}

bool clp::Identifier(const std::string& s)
{
    return IsIdentifier(s);
}

bool clp::Path(const std::string& s)
{
    return IsPath(s);
}


CommandDescriptorPtr clp::Command(
    const std::string& name,
    const std::string& description,
    const ParameterDescriptors& parameters,
    const OptionDescriptors& options)
{
    return std::make_shared<CommandDescriptor>(
        name,description,parameters,options);
}

OptionDescriptorPtr clp::Option(
    const std::string& name,
    const std::string& description,
    const ParameterDescriptors& parameters )
{
    return std::make_shared<OptionDescriptor>(
        name,description,parameters);
}

ParameterDescriptorPtr clp::Parameter(
    const std::string& name,
    const std::string& description,
    bool requirement,
    TypeChecker type_checker,
    Constraints constraints )
{
    return std::make_shared<ParameterDescriptor>(
        name,description,requirement,type_checker,constraints );
}

//-----------------------------------------------------------------------------

HasName::HasName( const std::string& s ) 
: name{s}
{
}

std::string_view HasName::getName() const 
{
    return name;
}

//-----------------------------------------------------------------------------

HasDescription::HasDescription(const std::string& s)
: description{s}
{
}

std::string_view HasDescription::getDescription() const
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

vcode ParameterDescriptor::validate(const std::string& p) 
{
    if (!type_checker(p))
        return vcode::param_invalid;
    for( auto& c : constraints )
    {
        if (!c->validate(p))
        {
            return vcode::param_invalid;
        }
    }
    return vcode::match;
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
    // Ensure that an optional parameter is not followed by one that is 
    // required. Also count the number of required parameters, for future
    // validation processes.
    numberOfRequiredParameters = 0;
    bool prev_optional{false};
    for( auto& p : parameter_descriptors )
    {
        if (p->parameterIsRequired())
        {
            numberOfRequiredParameters++;
            if ( prev_optional )
                throw std::invalid_argument(
                    "Optional parameters cannot be followed by one that "
                    "is required. Revisit your command descriptor "
                    "initialization.");
        }
        else
        {
            prev_optional = true;
        }
    }
}

const ParameterDescriptors& OptionDescriptor::getParameters() const
{
    return parameters;
}

vcode OptionDescriptor::validate( const std::vector<std::string>& arguments )
{
    if ( arguments.size() < numberOfRequiredParameters )
        return vcode::param_missing;
    if ( arguments.size() > parameters.size() )
        return vcode::param_invalid;

    for( int i{0}; i < arguments.size(); i++ )
    {
        vcode result {parameters[i]->validate(arguments[i])};
        if ( result != vcode::match )
            return result;
    }
    return vcode::match;
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

vcode CommandDescriptor::validate( const CommandLine& command_line )
{
    if (!command_line)
        return vcode::cmd_invalid;
    
    // Check command and its parameters
    // (OptionDescriptor refers to the base class here)
    vcode v{ OptionDescriptor::validate( command_line.getCommandParameters() ) };
    if ( v != vcode::match ) 
    {
        if ( v == vcode::opt_invalid ) 
            v = vcode::cmd_invalid;
        return v;
    }

    // Each option descriptor gets to see every option passed.
    //
    vcode result{};
    for( int i{0}; i < command_line.getOptionCount(); i++ )
    {
        auto& option_parameter{ command_line.getOptionParameters(i) };
        for( auto option_descriptor : options )
        {
            result = option_descriptor->validate( option_parameter );
            
            // Found a match, abort inner loop, and go on with
            // the next iteration
            if ( result == vcode::match )
                break;

            // Option name matched, but its parameters didnt fit.
            // Thus, the validation as a whole has failed.
            if ( result != vcode::opt_invalid )
                return result;
                
        }
    }
    return result;
}


//-----------------------------------------------------------------------------
