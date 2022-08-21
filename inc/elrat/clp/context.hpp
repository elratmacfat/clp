#ifndef ELRAT_CLP_CONTEXT_HPP
#define ELRAT_CLP_CONTEXT_HPP

#include <elrat/clp/desc.hpp>
#include <map>
namespace elrat {
namespace clp {

class desc_map;
using desc_map_ptr = std::shared_ptr<desc_map>;

class context;
using context_ptr = std::shared_ptr<context>;

// A descriptor map is a collection of command descriptors
//
class desc_map
: public desc
{
public:
    // desc constructor taking a "name" and a "description".
    using desc::desc; 

    // Throws if a command descriptor with the same name already exists
    void add( cmd_desc_ptr );

private:
    std::map<std::string,cmd_desc_ptr>  _cmd_descs;
};

class context
: public desc
{
public:
    // desc constructor taking a "name" and a "description".
    using desc::desc; 

    // 
    void add( desc_map_ptr );

private:
    std::vector<desc_map_ptr> _desc_maps;
};

} // clp
} // elrat

#endif

