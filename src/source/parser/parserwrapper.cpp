#include <stdexcept>

#include "elrat/clp/parserwrapper.hpp"

using namespace elrat::clp;


ParserWrapper::ParserWrapper( ParsingFunction func, const std::string& desc )
: parsing_function{func}
, syntax_description{desc}
{
    if (!parsing_function)
        throw std::runtime_error("Parsing Function cannot be nullptr.");
}

CommandLine ParserWrapper::parse(const std::string& input) const
{
    if ( parsing_function )
        return parsing_function(input);
    return CommandLine{};
}

const std::string& ParserWrapper::getSyntaxDescription() const
{
    return syntax_description;
}

