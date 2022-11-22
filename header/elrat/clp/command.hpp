#ifndef ELRAT_CLP_COMMAND_HPP
#define ELRAT_CLP_COMMAND_HPP

#include <memory>

#include <elrat/clp/commandline.hpp>

namespace elrat {
namespace clp {

class Command;
using CommandPtr = std::shared_ptr<Command>;


class Command
{
public:
    template <class COMMAND, class...ARGS>
    static CommandPtr Create(ARGS...args)
    {
        return std::make_shared<COMMAND>(args...);
    }

    virtual ~Command();
    virtual void execute(const CommandLine&) = 0;
};



}
}

#endif

