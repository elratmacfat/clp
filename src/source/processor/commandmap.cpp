#include "elrat/clp/commandmap.hpp"
#include "elrat/clp/errorhandling.hpp"
#include "commandwrapper.hpp"

using namespace elrat::clp;

void CommandMap::attach(const std::string& name, CommandPtr ptr)
{
    static const std::string where{"CommandMap::attach()"};
    throwIfEmpty(name, where);
    throwIfNull(ptr, where);
    
    std::vector<CommandPtr>& pointers = commands[name];
    for(auto& p : pointers)
        if (p == ptr)
            throw AlreadyInUseException("CommandMap::attach(): CommandPtr");
    
    pointers.push_back( ptr );
}

void CommandMap::detach(const std::string& name, CommandPtr ptr)
{
    throwIfEmpty(name,"CommandMap::detach()");

    std::vector<CommandPtr>& pointers{ commands[name] };
    if (ptr)
        for( auto current = pointers.begin(); current != pointers.end(); current++ )
            if ( *current == ptr )
            {
                pointers.erase(current);
                return;
            }
    else
        pointers.clear();
}

void CommandMap::invoke(const CommandLine& cmdline) const
{
    auto& key{ cmdline.getCommand() };
    auto& cmds{ this->find( key ) };
    for( auto cmd : cmds )
        cmd->execute(cmdline);
}

const std::vector<CommandPtr>& CommandMap::find(const std::string& name) const 
{
    if (commands.find(name) == commands.end())
        throw CommandNotFoundException(name);
    return commands.at(name);
}

void CommandMap::throwIfEmpty(const std::string& candidate, const std::string& where)
{
    if (!candidate.size())
        throw EmptyStringException(where);
}

void CommandMap::throwIfNull(CommandPtr p, const std::string& where )
{
    if (!p)
        throw NullptrAssignmentException(where);
}
