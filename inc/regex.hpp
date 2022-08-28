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

#endif

