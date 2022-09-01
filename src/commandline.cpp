#include <elrat/clp.hpp>
#include "modifiablecommandline.hpp"

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


