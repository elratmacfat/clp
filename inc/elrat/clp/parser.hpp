// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/parser.hpp
// Author.......: elratmacfat
// Description..: The Parser disassembles an issued command line into its
//                logical pieces (command, options and parameter).
//
#ifndef ELRAT_CLP_PARSER_HPP
#define ELRAT_CLP_PARSER_HPP

#include <elrat/clp/data.hpp>

#include <functional>
#include <memory>

namespace elrat {
namespace clp {

class parser {
public:
    template <class T, class...Args>
    static std::unique_ptr<parser> make(Args...);
    class error;
    virtual ~parser() {}
    virtual clp::data parse(const std::string&, parser::error&) = 0;
    virtual std::string_view syntax() const = 0;
};

template <class T, class...Args>
std::unique_ptr<parser> parser::make(Args...args) 
{
    return std::make_unique<T>(args...);
}

class parser::error 
{
public:
    error(
        const std::string& msg = "", 
        const std::string& xpr = "",
        const std::string& src = "");
    error(const error&) = default;
    error(error&&) = default;
    ~error() = default;
    error& operator=(const error&) = default;
    error& operator=(error&&) = default;
    std::string_view message() const;
    std::string_view expression() const;
    std::string_view source() const;
    operator bool() const;
private:
    std::string _msg;
    std::string _xpr;
    std::string _src;
};

class parser_wrapper
: public parser
{
public:
    using function = std::function<
        data( const std::string, parser::error& )>;

    parser_wrapper( parser_wrapper::function, const std::string& );
    clp::data parse(const std::string&, parser::error&);
    std::string_view syntax() const;
private:
    const std::string _syntax;
    const parser_wrapper::function _function;
};

class default_parser 
: public parser
{
public:
    clp::data parse(const std::string&,parser::error&);
    std::string_view syntax() const;
private:
    static const std::string _syntax;
};


} // namespace clp
} // namespace elrat

#endif 

