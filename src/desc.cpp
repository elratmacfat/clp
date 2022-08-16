#include <elrat/clp/desc.hpp>

using namespace elrat;

clp::desc::desc( const std::string& i, const std::string& d ) 
: _identifier{i}
, _description{d}
{

}

std::string_view clp::desc::identifier() const 
{
    return _identifier;
}

std::string_view clp::desc::description() const
{
    return _description;
}
