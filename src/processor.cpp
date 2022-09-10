#include <elrat/clp.hpp>

#include "elrat/clp/errorhandling.hpp"

using namespace elrat::clp;

Processor::Processor( std::shared_ptr<Parser> p )
: parser{p}
{
}


void Processor::attach(DescriptorMapPtr p)
{
    for( auto map : descriptor_maps )
        if ( map == p || map->getName() == p->getName() )
            throw AlreadyInUseException(p->getName() + "(Processor::attach())");
    descriptor_maps.push_back(p);
}

void Processor::attach(const std::string& name, CommandPtr ptr)
{
    command_map.attach(name,ptr);
}

bool Processor::process(const std::string& input) const
{
    CommandLine cmdline = parser->parse(input);

    for( auto current_desc_map : descriptor_maps )
    {
        if( current_desc_map->validate( cmdline ) )
        {
            auto& commands{ command_map.find( cmdline.getCommand() ) };
            for( auto command : commands )
                command->execute( cmdline );
            return true;
        }
    }
    return false;
}

