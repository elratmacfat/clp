// Project......: Command Line Processor (clp)
// File.........: elrat/clp.hpp
// Author.......: elratmacfat
// Description..: 
//
#ifndef ELRAT_CLP_HPP
#define ELRAT_CLP_HPP

#include <elrat/clp/parser.hpp>
#include <elrat/clp/descriptors.hpp>

std::ostream& operator<<(std::ostream&,const elrat::clp::CommandLine&);

std::ostream& operator<<(std::ostream&,const elrat::clp::ValidationResult&);

namespace elrat {
namespace clp {

    class CommandLineProcessor
    {
    };

} // clp
} // elrat

#endif

