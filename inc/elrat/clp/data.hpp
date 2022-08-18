// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/data.hpp
// Author.......: elratmacfat
// Description..: The 'data' class encapsulates the parsed elements of an 
//                input command line, and provides access through a meaningful
//                interface.
//
//                How an issued command line is interpreted is completely up
//                the concrete 'parser'-class, but the following presumptions
//                about the general structure of a command line are made (to
//                be able to provide a semantical interface):
//
//                - The command can have zero or more parameters.
//                - The command can have zero or more options.
//                - An option can have zero or more parameters.
//
//                A few notes to consider when using the 'data' class:
//
//                => Accessing non-existent elements will result in a 
//                   std::out_of_range exception. Therefore, boundaries should
//                   be checked first using 
//                      data::empty()
//                      data::cmd_param_count()
//                      data::opt_count()
//                      data::opt_param_count(opt_index)
//
//                   In addition the bool operator is overloaded, which 
//                   is an alternative to the empty() member, but checks
//                   the other way round:
//                      
//                      if ( data ) {
//                          std::cout << data.cmd(); // OK
//                          // Still need to test for parameter
//                          // and option count
//                      }
//
//                => The indices of command parameters, options and option's 
//                   parameters start at 1 (not zero, as would naturally be 
//                   expected):
//
//                      int n = data.cmd_param_count(); // n = 2
//                      auto p1 = data.cmd_param(1);
//                      auto p2 = data.cmd_param(2);
//
//                      int m = data.opt_count(); // m = 1;
//                      auto opt_name = data.opt(1); //
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
    operator bool() const;

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
