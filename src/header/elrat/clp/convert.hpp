#ifndef ELRAT_CLP_CONVERT_HPP
#define ELRAT_CLP_CONVERT_HPP

#include <sstream>

namespace elrat {
namespace clp {

template <class T> T convert(const std::string&);
template <> const char* convert<const char*>(const std::string&);

// --- 
template <class T> 
T convert(const std::string& arg) 
{
    std::stringstream ss;
    ss << arg;
    T result;
    ss >> result;
    return result;
}

} // clp
} // elrat

#endif

