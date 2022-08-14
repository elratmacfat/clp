#ifndef ELRAT_CLP_COMMON_HPP
#define ELRAT_CLP_COMMON_HPP

#include <regex>
#include <string>
#include <vector>

namespace elrat {
namespace clp {

// These are considered negative: -5, -090.1, -.1234
bool is_negative( const std::string& s );

// if argument starts with a dash and is not a negative, false is returned.
bool is_parameter( const std::string& s );

} // clp
} // elrat

#endif

