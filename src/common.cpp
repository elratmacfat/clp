#include "elrat/clp/common.hpp"

bool elrat::clp::is_negative( const std::string& s )
{
    // Regex matches:
    // -5
    // -090.134
    // -.1234
    std::regex rgx("^(-\\d*.?\\d+))$");
    return std::regex_match( s, rgx );
}

bool elrat::clp::is_parameter( const std::string& s )
{
    std::regex rgx("^(-.+)$");
    bool dash{std::regex_match(s,rgx)};
    return !( dash && !elrat::clp::is_negative(s) ); 
}

