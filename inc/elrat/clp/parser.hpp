// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/parser.hpp
// Author.......: elratmacfat
// Description..: The Parser disassembles an issued command line into its
//                logical pieces (command, options and parameter).
//
//                This file contains the parser interface, which can be used 
//                by the application programmer to define custom parsers with
//                custom syntax rules.
//
//                Each parser class has to write its results into a
//                parser::data object. If an error occurs while parsing 
//                a command line, the parser::error class can be used to 
//                communicate the error type back the application.
//
#ifndef ELRAT_CLP_PARSER_HPP
#define ELRAT_CLP_PARSER_HPP

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace elrat {
namespace clp {
    
    class parser {
    public:
        class data;
        class error;
        
        template <class T, class...Args>
        static std::unique_ptr<parser> make(Args...args) 
        {
            return std::make_unique<T>(args...);
        }
    
        virtual ~parser() 
        {
        }
    
        virtual data parse(const std::string&, error&) = 0;
        virtual std::string_view syntax() const = 0;
    };
    
    class parser::data {
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
        
        const std::vector<std::string>& cmd_param_vec() const;
        const std::vector<std::string>& opt_param_vec(int i) const;
    private:
        structure _s;
    };
    
    class parser::error 
    {
    public:
        enum class code 
        {
            success,
            failure,
            no_input,
            exception,
            syntax,
            redundant   // indicates multiple use of the same option.
        };
    
        error(
            code c = code::success,
            const std::string& msg = "",
            const std::string& src = "");
        error(const error&) = delete;
        error(error&&) = default;
        ~error() = default;
        error& operator=(const error&) = delete;
        error& operator=(error&&) = default;
        code operator()() const;
        std::string_view message() const;
        std::string_view source() const;
        operator bool() const;
    private:
        code        _code;
        std::string _msg;
        std::string _src;
    };
    
    class parser_wrapper
    : public parser
    {
    public:
        using function = std::function<
            data( const std::string, error& )>;
    
        parser_wrapper( function, const std::string& );
        data parse(const std::string&, error&);
        std::string_view syntax() const;
    private:
        const std::string   _syntax;
        const function      _function;
    };
    
    class native_parser 
    : public parser
    {
    public:
        data parse(const std::string&,parser::error&);
        std::string_view syntax() const;
    private:
        static const std::string _syntax;
    };
    

} // namespace clp
} // namespace elrat

std::ostream& operator<<(std::ostream&,const elrat::clp::parser::data&);

std::ostream& operator<<(std::ostream&,const elrat::clp::parser::error&);

std::ostream& operator<<(std::ostream&,const elrat::clp::parser::error::code&);

#endif 

