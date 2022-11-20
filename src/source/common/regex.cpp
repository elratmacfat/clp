#include "common/regex.hpp"
#include <regex>

RegEx::RegEx(const std::string& regular_expression)
: regex(regular_expression)
{
}

bool RegEx::operator()(const std::string& candidate) const
{
    return std::regex_match( candidate, this->regex );
}

const RegEx IsDecimal(
    "[\\+-]?\\d+"
);

const RegEx IsPositiveDecimal(
    "\\+?\\d+"
);

const RegEx IsNegativeDecimal(
    "-\\d+"
);

const RegEx IsHexaDecimal(
    "0[xX][0-9a-fA-F]+"
);

const RegEx IsFloatingPoint(
    "[\\+-]?\\d*\\.?\\d+"
);

const RegEx IsName(
    "[_a-zA-Z]+[\\w-]*\\w+"
);

const RegEx IsIdentifier(
    "[_a-zA-Z]\\w+"
);

const RegEx IsWindowsPath(
    "([a-zA-Z]:(\\\\[\\w\\-_\\. ])?)?"  // C: | C:\ | C:\some-thing
    "(\\\\?[\\w\\-_\\. ]+)*\\\\?"       // \some-thing\ | something.exe
);
const RegEx IsUnixPath(
    "(/?[\\w\\-_(\\.\\.?) ]+)*/?"
);

const RegEx IsEmailAddress(
    "\\w+(\\.\\w+)*(\\-\\w+)*@"     // user@ 
    "\\w+(\\.\\w+)*(\\-\\w+)*\\."   // domain
    "\\w+"                          // suffix
);


