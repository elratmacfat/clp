#include "elrat/clp.hpp"
#include "elrat/clp/errorhandling.hpp"
#include "commandwrapper.hpp"

using namespace elrat::clp;

Processor::Processor( std::shared_ptr<Parser> p )
: parser{p}
{
}

void Processor::attach(CommandDescriptorPtr p)
{
    descriptors.attach(p);
}

void Processor::attach(CommandDescriptorPtr desc, CommandPtr cmd )
{
    attach(desc);
    attach(desc->getName(), cmd);
}

void Processor::attach(
    CommandDescriptorPtr desc, 
    std::function<void(const CommandLine&)> cmd )
{
    attach( desc );
    attach( desc->getName(), cmd );
}

void Processor::attach(const std::string& name, CommandPtr ptr)
{
    commands.attach(name,ptr);
}

void Processor::attach(
    const std::string& name, 
    std::function<void(const CommandLine&)> function)
{
    commands.attach(name, Command::Create<CommandWrapper>(function));
}

void Processor::process(const std::string& input) const
{
    CommandLine cmdline = parser->parse( input );
    if ( descriptors.validate( cmdline ) )
        commands.invoke( cmdline );
    else
        throw InvalidCommandException( cmdline.getCommand() );
}

