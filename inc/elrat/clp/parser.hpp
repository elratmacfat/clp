#ifndef ELRAT_CLP_PARSER_HPP
#define ELRAT_CLP_PARSER_HPP

#include "elrat/clp/parser_interface.hpp"

namespace elrat {
namespace clp {

class Parser : public ParserInterface
{
public:
    
    Parser( std::shared_ptr<InputInterface> );
    
    Parser(const Parser&)=delete;
    
    Parser(Parser&&)=default;
    
    ~Parser()=default;
    
    Parser& operator=(const Parser&)=delete;
    
    Parser& operator=(Parser&&)=default;


    // ParserInterface
    //

    std::string_view getSyntaxDescription() const;

    void parse(const std::string& chunk);
    
    bool done() const;
    
    std::shared_ptr<InputReaderInterface> getResult();

private:

    enum class State {
        Empty,          
        ParsedCommand, 
        ParsedOption, 
        Locked
    };

    std::shared_ptr<InputInterface> _result;

    State _state;

    static const std::string _syntax;
};


} // namespace clp
} // namespace elrat

#endif 

