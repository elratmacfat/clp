// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/util.hpp
// Author.......: elratmacfat
// Description..: Utility functions
//
#ifndef ELRAT_CLP_UTIL_HPP
#define ELRAT_CLP_UTIL_HPP

#include <sstream>

namespace elrat {
namespace clp {
namespace util {

template <class T>
T convert(const std::string& arg) 
{
    std::stringstream ss(arg);
    T t;
    ss >> t;
    return t;
}

} // util
} // clp
} // elrat

#endif

