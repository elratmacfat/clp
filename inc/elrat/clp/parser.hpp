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

    void parse(const std::string& chunk);
    
    bool done() const;
    
    std::shared_ptr<InputReaderInterface> getResult();

private:

    enum class State {
        CMD,        // waiting for command
        CMD_PARAM,  // command has been set, awaiting cmd param or option
        OPT_PARAM,  // option has been passed, next opt param or another option
        LOCKED
    };

    std::shared_ptr<InputInterface> _result;

    State _state;
};


} // namespace clp
} // namespace elrat

#endif 

