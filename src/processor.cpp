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

bool Processor::process(const std::string& input) const
{
   return true;
}

