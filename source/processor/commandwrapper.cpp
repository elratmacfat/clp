#include "commandwrapper.hpp"
#include "elrat/clp/errorhandling.hpp"

using elrat::clp::CommandLine;

CommandWrapper::CommandWrapper( Function f )
: function(f)
{
    if (!function)
        throw elrat::clp::NullptrAssignmentException("CommandWrapper(Function f)");
}

void CommandWrapper::execute(const CommandLine& cmdline)
{
    function(cmdline);
}


