// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/data.hpp
// Author.......: elratmacfat
// Description..: 
//
// - The command itself is the first word of the input.
// - The command can have zero or more parameters.
// - The command can have zero or more options.
// - An option can have zero or more parameters.
//
// Note that indexing starts with 1, not zero!
//
//      int n = data.cmd_param_count(); // n = 2
//      auto p1 = data.cmd_param(1);
//      auto p2 = data.cmd_param(2);
//
//      int m = data.opt_count(); // m = 1;
//      auto opt_name = data.opt(1); //
//
#ifndef ELRAT_CLP_DATA_HPP
#define ELRAT_CLP_DATA_HPP

#include <iostream>
#include <string_view>
#include <vector>

namespace elrat {
namespace clp {

class data {
public:
    using structure = std::vector<std::vector<std::string>>;
    data() = default;
    data( structure&& );
    data(const data&) = default;
    data(data&&) = default;
    ~data() = default;
    data& operator=(const data&) = default;
    data& operator=(data&&) = default;

    void clear();
    bool empty() const;

    int cmd_param_count() const;
    int opt_count() const;
    int opt_exists(const std::string&) const;
    int opt_param_count(int) const;
    
    std::string_view cmd() const;
    std::string_view cmd_param(int=1) const;
    std::string_view opt(int=1) const;
    std::string_view opt_param(int=1,int=1) const;

private:
    structure _s;
};


} // namespace clp
} // namespace elrat


#endif // include guard
