#ifndef ELRAT_CLP_REGEX_HPP
#define ELRAT_CLP_REGEX_HPP

#include <string>

namespace elrat {
namespace clp {
namespace regex {

bool is_decimal( const std::string& );
bool is_positive_decimal( const std::string& );
bool is_negative_decimal( const std::string& );
bool is_hexadecimal( const std::string& );
bool is_octal( const std::string& );
bool is_floating_point( const std::string& );
bool is_name( const std::string& );
bool is_identifier( const std::string& );

} // regex
} // clp
} // elrat


#endif

