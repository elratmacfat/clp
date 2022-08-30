// Project......: Command Line Processor (clp)
// File.........: elrat/clp.hpp
// Author.......: elratmacfat
// Description..: 
//
#ifndef ELRAT_CLP_HPP
#define ELRAT_CLP_HPP

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace elrat {
namespace clp {
    
    struct CommandLine
    {
        using Parameters = std::vector<std::string>;
        using Options = std::vector<std::pair<std::string,Parameters>>;
        std::string command;
        Parameters  parameters;
        Options     options;
    };
    
    class Parser
    {
    public:
        virtual ~Parser();
        virtual CommandLine parse( const std::string& ) const = 0;
        virtual const std::string& getSyntaxDescription() const = 0;
    };
   
    class ParserWrapper
    {
    public:
        using ParsingFunction = std::function<CommandLine(const std::string&)>;
        ParserWrapper( ParsingFunction, const std::string& description );
        virtual CommandLine parse(const std::string&) const;
        virtual const std::string& getSyntaxDescription() const;
    private:
        ParsingFunction parsing_function;
        std::string syntax_description;
    };

    class NativeParser
    : public Parser
    {
    public:
        class Exception;
        virtual CommandLine parse( const std::string& ) const;
        virtual const std::string& getSyntaxDescription() const;
    private:
        static const std::string SyntaxDescription;
    };
    
} // clp
} // elrat

std::ostream& operator<<(std::ostream&,const elrat::clp::CommandLine&);

#endif

