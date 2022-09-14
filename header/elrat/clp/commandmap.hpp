#ifndef ELRAT_CLP_COMMANDMAP_HPP
#define ELRAT_CLP_COMMANDMAP_HPP

#include <elrat/clp/commandline.hpp>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace elrat {
namespace clp {

class Command;
using CommandPtr = std::shared_ptr<Command>;


class Command
{
public:
    template <class COMMAND, class...ARGS>
    static CommandPtr Create(ARGS...args)
    {
        return std::make_shared<COMMAND>(args...);
    }

    virtual ~Command();
    virtual void execute(const CommandLine&) = 0;
};

class CommandMap 
{
public:
    void attach(const std::string& name, CommandPtr);
    void detach(const std::string& name, CommandPtr = nullptr);
    const std::vector<CommandPtr>& find(const std::string&) const;
private:
    std::map<std::string, std::vector<CommandPtr>> commands;

    void throwIfEmpty(const std::string& candidate, const std::string& where);
    void throwIfNull(CommandPtr candidate, const std::string& where);
};

} // clp
} // elrat

#endif

