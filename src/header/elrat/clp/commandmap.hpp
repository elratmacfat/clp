#ifndef ELRAT_CLP_COMMANDMAP_HPP
#define ELRAT_CLP_COMMANDMAP_HPP

#include <elrat/clp/commandline.hpp>
#include <elrat/clp/command.hpp>

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace elrat {
namespace clp {

class CommandMap 
{
public:
    void attach(const std::string& name, CommandPtr);
    void detach(const std::string& name, CommandPtr = nullptr);
    void invoke(const CommandLine&) const;
private:
    std::map<std::string, std::vector<CommandPtr>> commands;

    void throwIfEmpty(const std::string& candidate, const std::string& where);
    void throwIfNull(CommandPtr candidate, const std::string& where);
    const std::vector<CommandPtr>& find(const std::string&) const;
};

} // clp
} // elrat

#endif

