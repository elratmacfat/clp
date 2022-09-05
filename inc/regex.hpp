#ifndef REGEX_HPP
#define REGEX_HPP

#include <regex>
#include <string>

class RegEx
{
public:
    RegEx(const std::string& regular_expression);
    bool operator()(const std::string& candidate) const;
private:
    std::regex regex;
};

extern const RegEx IsDecimal;
extern const RegEx IsPositiveDecimal;
extern const RegEx IsHexaDecimal;
extern const RegEx IsFloatingPoint;
extern const RegEx IsName;
extern const RegEx IsIdentifier;
extern const RegEx IsWindowsPath;
extern const RegEx IsUnixPath;

#endif

