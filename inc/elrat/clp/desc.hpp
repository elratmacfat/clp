// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/desc.hpp
// Author.......: elratmacfat
// Description..:
//
#ifndef ELRAT_CLP_DESC_HPP
#define ELRAT_CLP_DESC_HPP

#include <string>
#include <string_view>

namespace elrat {
namespace clp {

class desc 
{
public:
    desc() = delete;
    desc( 
        const std::string& i, 
        const std::string& d );
    desc(const desc&) = default;
    desc(desc&&) = default;
    ~desc() = default;
    desc& operator=(const desc&) = default;
    desc& operator=(desc&&) = default;
    std::string_view identifier() const;
    std::string_view description() const;
private:
    std::string _identifier;
    std::string _description;
};

class param_desc 
: public desc
{
public:
    param_desc(
        const std::string& identifier,

        const std::string& description= "" );
};

class opt_desc 
: public desc
{

};

class cmd_desc
: public opt_desc
{

};

} // namespace clp
} // namespace elrat

#endif // include guard
