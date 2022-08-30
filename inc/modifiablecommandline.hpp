#ifndef MODIFIABLECOMMANDLINE_HPP
#define MODIFIABLECOMMANDLINE_HPP

#include <elrat/clp.hpp>

class ModifiableCommandLine
: public elrat::clp::CommandLine
{
public:
    void setCommand(const std::string&);
    void addParameter(const std::string&);
    void addOption(const std::string&);
    void addOptionParameter(const std::string&);
};

#endif

