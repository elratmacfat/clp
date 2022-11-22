#ifndef COMMANDWRAPPER_HPP
#define COMMANDWRAPPER_HPP

#include <functional>
#include "elrat/clp/processor.hpp"

class CommandWrapper
: public elrat::clp::Command
{
public:
    using Function = std::function<void(const elrat::clp::CommandLine&)>;
    CommandWrapper( Function );
    virtual void execute(const elrat::clp::CommandLine&);
private:
    Function function;
};

#endif

