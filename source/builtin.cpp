
#include "builtin.hpp"

#include <cstdlib>

using namespace elrat::clp;

HelpCommand::HelpCommand(
    const std::vector<DescriptorMapPtr>& v,
    std::ostream* p )
: descriptor_maps{v}
, os{p}
{
}

void HelpCommand::setOutputStream(std::ostream* p)
{
    os = p;
}

void HelpCommand::execute(const CommandLine& cmdline)
{
    for( DescriptorMapPtr map : descriptor_maps )
        printDescriptorMap( *os, map );
}

CommandDescriptorPtr HelpDescriptor::descriptor;

CommandDescriptorPtr HelpDescriptor::Create()
{
    if(!descriptor)
    {
        descriptor = CommandDescriptor::Create(
            "help","Built-in help command."
        );
    }
    return descriptor;
}

void ExitCommand::execute(const CommandLine& cmdline)
{
    std::exit(0);
}

CommandDescriptorPtr ExitDescriptor::descriptor;

CommandDescriptorPtr ExitDescriptor::Create()
{
    if (!descriptor)
    {
        descriptor = CommandDescriptor::Create(
            "exit", "Exit program."
        );
    }
    return descriptor;
}

void printDescriptorMap(std::ostream& os, DescriptorMapPtr p)
{
    auto& map_name{ p->getName() };
    os << map_name << '\n';
    for( int i{0}; i<map_name.size(); i++)
        os << '-';
    os << '\n';
    auto& descriptors{ p->getCommandDescriptors() };
    for( auto descriptor: descriptors )
    {
        os << descriptor->getName() << '\n';
    }
}
