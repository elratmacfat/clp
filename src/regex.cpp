#include "elrat/clp/regex.hpp"

#include <regex>

namespace elrat::clp::regex 
{
static bool rgx( const std::string& s, const std::string& r )
{
    return std::regex_match(s,std::regex(r));
}

bool is_decimal( const std::string& s )
{
    return rgx(s,"[\\+-]?\\d+");
}

bool is_positive_decimal( const std::string& s ) 
{
    return rgx(s,"\\+?\\d+");
}

bool is_negative_decimal( const std::string& s )
{
    return rgx(s,"-\\d+");
}

bool is_hexadecimal( const std::string& s )
{
    return rgx(s,"0[xX][0-9a-fA-F]+");
}

bool is_floating_point( const std::string& s )
{
    return rgx(s,"[\\+-]?\\d*\\.?\\d+");
}

bool is_name( const std::string& s )
{
    return rgx(s,"[_a-zA-Z]+[\\w-]*\\w+");
}

bool is_identifier( const std::string& s )
{
    return rgx(s,"[_a-zA-Z]\\w+");
}



} // namespace
