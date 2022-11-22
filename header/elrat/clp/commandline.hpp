#ifndef ELRAT_CLP_COMMANDLINE_HPP
#define ELRAT_CLP_COMMANDLINE_HPP

#include <elrat/clp/convert.hpp>

#include <ostream>
#include <string>
#include <vector>

namespace elrat {
namespace clp {

class CommandLine
{
public:
    using Parameters = std::vector<std::string>;
    using Options = std::vector<std::pair<std::string,Parameters>>;
    
    operator bool() const;
    
    const std::string& getCommand() const;
    
    bool optionExists(const std::string&) const;
    int getOptionCount() const;
    const std::string& getOption(int) const;
    
    const Parameters& getCommandParameters() const;
    const std::string& getCommandParameter(int) const;
    const Parameters& getOptionParameters(const std::string&) const;
    const Parameters& getOptionParameters(int) const;

    template <class T> T getCommandParameterAs(int) const;
    template <class T> T getOptionParameterAs(const std::string&,int) const;
    template <class T> T getOptionParameterAs(int,int) const;

    void setCommand(const std::string&);
    void addCommandParameter(const std::string&);
    void addOption(const std::string&);
    void addOptionParameter(const std::string&); // last inserted option
private:
    std::string command;
    Parameters  parameters;
    Options     options;
};

template <class T> 
T CommandLine::getCommandParameterAs(int param_index) const
{
    return convert<T>( parameters.at(param_index) );
}

template <class T> 
T CommandLine::getOptionParameterAs(const std::string& opt_name,int param_index) const
{
    int opt_index{-1};
    for( int i{0}; i < options.size(); i++)
        if ( options[i].first == opt_name )
        {
            opt_index = i;
            break;
        }
    return getOptionParameterAs<T>(opt_index,param_index);
}

template <class T> 
T CommandLine::getOptionParameterAs(int opt_index, int param_index) const
{
    return convert<T>( options.at(opt_index).second.at(param_index) );
}

} // clp
} // elrat

std::ostream& operator<<(std::ostream&,const elrat::clp::CommandLine&);

#endif 

