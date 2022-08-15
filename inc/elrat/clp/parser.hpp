// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/parser.hpp
// Author.......: elratmacfat
// Description..:
//
#ifndef ELRAT_CLP_PARSER_HPP
#define ELRAT_CLP_PARSER_HPP

#include <functional>
#include <memory>
#include <string_view>
#include <vector>

namespace elrat {
namespace clp {

class Parser
{
public:
    using Data = std::vector<std::vector<std::string>>;
    using Strategy = Data(*)(const std::string&);

    Parser( Strategy = Parser::DefaultStrategy );
    Parser(const Parser&) = default;
    Parser(Parser&&) = default;
    ~Parser() = default;
    Parser& operator=(const Parser&) = default;
    Parser& operator=(Parser&&) = default;

    bool parse(const std::string&);
    void clear();
    std::string_view getLastError() const;

    operator bool() const;

    int getCommandParameterCount() const;
    int getOptionCount() const;
    int getOptionIndex(const std::string&) const;
    int getOptionParameterCount(int) const;
    
    std::string_view getCommand() const;
    std::string_view getCommandParameter(int=0) const;
    std::string_view getOption(int=0) const;
    std::string_view getOptionParameter(int,int=0) const;

    const Data& data() const;
private:
    std::string _errmsg;
    Data _data;
    Strategy _strategy;
    static Data DefaultStrategy(const std::string&); 
};

std::ostream& operator<<(std::ostream&,const Parser&);

} // namespace clp
} // namespace elrat

#endif 

