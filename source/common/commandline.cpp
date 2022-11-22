#include <sstream>
#include <stdexcept>

#include "elrat/clp/commandline.hpp"

using namespace elrat::clp;
using Parameters = CommandLine::Parameters;

CommandLine::operator bool() const 
{
    return ( command.size() > 0 );
}

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

const std::string& CommandLine::getCommandParameter(int index) const
{
    return parameters.at(index);
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

std::ostream& operator<<(std::ostream& os, const elrat::clp::CommandLine& cl)
{
    static auto print_parameters{
        [&os](bool indent, const std::vector<std::string>& vec)
        {
            for( int i{0}; i < vec.size(); i++ )
            {
                if (indent)
                    os << "  ";
                os << "-> Parameter #" << i;
                if (!indent)
                    os << "..";
                os << "..: \"" << vec[i] << "\"\n";
            }
        }
    };
    if (!cl.getCommand().size()) 
        return os;
    
    os << "Command............: \""
        << cl.getCommand()
        << "\"\n";
    print_parameters( false, cl.getCommandParameters() );
    
    int count = cl.getOptionCount();
    for( int i{0}; i < count; i++ )
    {        
        os << "-> Option" 
            << "..........: \""
            << cl.getOption(i)
            << "\"\n";
        print_parameters(true, cl.getOptionParameters(i));
    }
    return os;
}


