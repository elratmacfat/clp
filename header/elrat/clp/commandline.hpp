#ifndef ELRAT_CLP_COMMANDLINE_HPP
#define ELRAT_CLP_COMMANDLINE_HPP

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
    const Parameters& getOptionParameters(const std::string&) const;
    const Parameters& getOptionParameters(int) const;
    void setCommand(const std::string&);
    void addCommandParameter(const std::string&);
    void addOption(const std::string&);
    void addOptionParameter(const std::string&); // last inserted option
private:
    std::string command;
    Parameters  parameters;
    Options     options;
};

} // clp
} // elrat

#endif 

