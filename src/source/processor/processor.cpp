#include "elrat/clp/processor.hpp"
#include "elrat/clp/errorhandling.hpp"

#include "commandwrapper.hpp"
#include "builtin.hpp"

using namespace elrat::clp;

Processor::Processor( std::shared_ptr<Parser> p )
: parser{p}
{
    auto builtin_descriptors{ DescriptorMap::Create("Built-In Commands") };
    descriptor_maps.push_back( builtin_descriptors );
    addHelpCommand();
    addExitCommand();
}

void Processor::addExitCommand()
{
    auto builtin_descriptors{ descriptor_maps[0] };
    auto exit_descriptor{ ExitDescriptor::Create() };
    builtin_descriptors->attach( exit_descriptor );
    auto exit_command{ Command::Create<ExitCommand>() };
    commands.attach( exit_descriptor->getName(), exit_command );
}

void Processor::addHelpCommand()
{
    auto builtin_descriptors{ descriptor_maps[0] };
    auto help_descriptor{ HelpDescriptor::Create() };
    builtin_descriptors->attach( help_descriptor );
    auto help_command{ std::make_shared<HelpCommand>( descriptor_maps ) };
    commands.attach( help_descriptor->getName(), help_command );
}


void Processor::attach(CommandDescriptorPtr p)
{
    if ( descriptor_maps.size() < 2 )
        descriptor_maps.push_back( DescriptorMap::Create("Commands") );
    descriptor_maps.back()->attach(p);
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
    for( auto map : descriptor_maps )
    {
        if ( map->validate( cmdline ) )
        {
            commands.invoke( cmdline );
            return;
        }
    }
    throw InvalidCommandException( cmdline.getCommand() );
}

