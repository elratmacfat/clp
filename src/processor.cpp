#include <elrat/clp.hpp>

#include "errorhandling.hpp"

using namespace elrat::clp;

Processor::Processor( std::shared_ptr<Parser> p )
: parser{p}
{
}

void Processor::setParser( std::shared_ptr<Parser> p ) 
{
    parser = p;
}

void Processor::attachDescriptor(CommandDescriptorPtr p )
{
    if ( !p )
        throwNullPointerAssignmentException("Processor::attachDescriptor");
    
    for( auto& cd : command_descriptors )
        if ( cd->getName() == p->getName() )
            throwAlreadyInUseException(p->getName());

    command_descriptors.push_back( p );
}

void Processor::attachCommand(const std::string& name, CommandPtr p)
{
    if (!p)
        throwNullPointerAssignmentException("Processor::attachCommand");
    
    std::vector<CommandPtr>& command_pointers = commands[name];

    for( auto ptr : command_pointers )
        if ( ptr == p )
            throwAlreadyInUseException("Processor::attachCommand");
    
    command_pointers.push_back(p);
}

bool Processor::process(const std::string& input) const
{
    CommandLine command_line{ parser->parse(input) };
    
    bool is_valid{ validateCommandLine(command_line) };
    if ( is_valid )
    {
        invokeCommand(command_line);
    }
    return is_valid;
}

bool Processor::validateCommandLine(const CommandLine& command_line) const
{
    ValidationResult result{ValidationResult::InvalidCommand};
    for( auto& cd : command_descriptors )
    {
        result = cd->validate(command_line);
        if ( isMatch(result) )
            return true;
    }
    return false;
}

bool Processor::invokeCommand(const CommandLine& command_line) const
{
    const std::vector<CommandPtr>& pointers = commands.at(command_line.getCommand());
    if (pointers.size() == 0)
        return false;
    for( auto ptr : pointers )
            ptr->execute(command_line);
    return true;
}

