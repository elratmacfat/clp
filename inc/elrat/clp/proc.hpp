#ifndef ELRAT_CLP_PROC_HPP
#define ELRAT_CLP_PROC_HPP

#include <elrat/clp/context.hpp>
#include <elrat/clp/desc.hpp>
#include <elrat/clp/parser.hpp>
#include <elrat/clp/cmd.hpp>

namespace elrat {
namespace clp {

class proc
{
public:
    proc() = default;
    proc(const proc&) = default;
    proc(proc&&) = default;
    ~proc() = default;
    proc& operator=(const proc&) = default;
    proc& operator=(proc&&) = default;

    // The command descriptor's name must be unique.
    //
    void add_command( cmd_desc_ptr );

private:
    std::vector<context>            _contexts;
    std::vector<desc_map_ptr>       _cmd_desc_maps;
    std::map<std::string,cmd_ptr>   _cmds;
};

} // clp
} // elrat

#endif

