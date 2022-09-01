#include <stdexcept>

#include <elrat/clp.hpp>

using elrat::clp::CommandLine;
using Parameters = elrat::clp::CommandLine::Parameters;

//
// Implementation of class CommandLine defined in the public header
//

const std::string& CommandLine::getCommand() const 
{
    return command;
}

bool CommandLine::optionExists(const std::string& option_name) const
{
    for( auto opt : options )
        if ( opt.first == option_name )
            return true;
    return false;
}

int CommandLine::getOptionCount() const 
{
    return options.size();
}

const std::string& CommandLine::getOption(int index) const 
{
    return options.at(index).first;
}

const Parameters& CommandLine::getCommandParameters() const 
{
    return parameters;
}

const Parameters& CommandLine::getOptionParameters(const std::string& option_name ) const
{
    for( auto& opt: options )
        if (opt.first == option_name)
            return opt.second;
    throw std::invalid_argument("getOptionParameter: Option not found.");
}

const Parameters& CommandLine::getOptionParameters(int index) const 
{
    return options.at(index).second;
}


void CommandLine::setCommand(const std::string& command_name)
{
    command = command_name;
}

void CommandLine::addCommandParameter(const std::string& parameter)
{
    parameters.push_back(parameter);
}

void CommandLine::addOption(const std::string& option_name)
{
    options.push_back( std::make_pair(option_name, Parameters{}) );
}

void CommandLine::addOptionParameter(const std::string& parameter)
{
    if (!options.size())
        throw std::runtime_error("addOptionParameter: No option added yet.");
    options.back().second.push_back(parameter);
}


