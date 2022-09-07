#include "elrat/clp/commands.hpp"
#include "errorhandling.hpp"

using namespace elrat::clp;

Command::~Command()
{
}

CommandWrapper::CommandWrapper( Function f )
: function(f)
{
    if (!function)
        ThrowException::NullPointerAssignment("CommandWrapper(Function f)");
}

void CommandWrapper::execute(const CommandLine& cmdline)
{
    function(cmdline);
}

void Commands::attach(const std::string& name, CommandPtr ptr)
{
    static const std::string where{"Commands::attach()"};
    throwIfEmpty(name, where);
    throwIfNull(ptr, where);
    
    std::vector<CommandPtr>& pointers = commands[name];
    for(auto& p : pointers)
        if (p == ptr)
            ThrowException::AlreadyInUse("CommandPtr");
    
    pointers.push_back( ptr );
}

void Commands::detach(const std::string& name, CommandPtr ptr)
{
    throwIfEmpty(name,"Commands::detach()");

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

const std::vector<CommandPtr>& Commands::find(const std::string& name) const 
{
    if (commands.find(name) == commands.end())
        ThrowException::CommandNotFound(name);
    return commands.at(name);
}

void Commands::throwIfEmpty(const std::string& candidate, const std::string& where)
{
    if (!candidate.size())
        ThrowException::EmptyStringAssignment(where);
}

void Commands::throwIfNull(CommandPtr p, const std::string& where )
{
    if (!p)
        ThrowException::NullPointerAssignment(where);
}
