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

// ------ Parser (Top-Level class) --------------------------------------------
class Parser
{
public:
    
    class Algorithm;    // Interface (Strategy pattern)
    class AlgWrapper;   // Wraps around a stateless algorithm function
    class Alg20;        //
    class Alg22;        //
    
    class Error;

    using Data = std::vector<std::vector<std::string>>;
    using Function = Data(*)(const std::string&);

private:
    static Data LegacyFunction(const std::string&); 

public:

    Parser( std::unique_ptr<Algorithm>&& );

    Parser( Function=Parser::LegacyFunction,
        const std::string& ="<cmd> [<param>]* [<[--option>|<-o>][<param>]*]*");
    
    Parser(const Parser&) = default;
    Parser(Parser&&) = default;
    ~Parser() = default;
    Parser& operator=(const Parser&) = default;
    Parser& operator=(Parser&&) = default;

    Error parse(const std::string&);
    void clear();
    std::string_view getSyntaxDescription() const;
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
    Data _data;
    std::unique_ptr<Algorithm> _algorithm;
};


// ------ Parser::Algorithm (Interface, Strategy Pattern ) --------------------
class Parser::Algorithm
{
public:
    template <class A, class...Args>
    static std::unique_ptr<Algorithm> Use(Args...args) 
    {
        return std::make_unique<A>(args...);
    }

    virtual ~Algorithm() { }
    virtual std::string_view syntax() const = 0;
    virtual Parser::Data parse( const std::string&, Parser::Error& ) = 0;
};

// ------ Parser::AlgWrapper (Concrete Strategy) ------------------------------
class Parser::AlgWrapper : public Parser::Algorithm
{
public:
    AlgWrapper( Parser::Function, const std::string& );
    virtual std::string_view syntax() const;
    virtual Parser::Data parse( const std::string&, Parser::Error& );
private:
    Parser::Function _function;
    const std::string _syntax;
};

// ------ Parser::Alg20 (Algorithm, Concrete Strategy) ------------------------
class Parser::Alg20 : public Parser::Algorithm
{
public:
    std::string_view syntax() const;
    Parser::Data parse( const std::string&, Parser::Error& );
private:
    static const std::string _syntax;
};

// ------ Parser::Alg22 (Algorithm, Concrete Strategy) ------------------------
class Parser::Alg22 : public Parser::Algorithm
{
public:
    std::string_view syntax() const;
    Parser::Data parse( const std::string&, Parser::Error& );
private:
    static const std::string _syntax;
};

// ------ Parser::Error -------------------------------------------------------
class Parser::Error
{
public:
    Error() = default;
    Error(const std::string&);

    operator bool() const;
    
    std::string_view source() const;
    std::string_view what() const;
    std::string_view expression() const;
    
    void clear();
    void source(const std::string&);
    void set(
        const std::string& what, 
        const std::string& expression = std::string());
    
private:
    std::string _source;
    std::string _expression;
    std::string _what;
};


// ------------------ Free Functions ------------------------------------------

std::ostream& operator<<(std::ostream&,const Parser&);

std::ostream& operator<<(std::ostream&,const Parser::Error&);

} // namespace clp
} // namespace elrat

#endif 

