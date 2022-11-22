#include "elrat/clp/nativeparser.hpp"
#include "elrat/clp/errorhandling.hpp"

#include "tokenhandler.hpp"

#include <regex>

using namespace elrat::clp;

static std::vector<std::string> tokenize(const std::string& input)
{
    std::vector<std::string> result{};
    std::regex separating_regex("(\"[^\"]+\")|([^\\s=]+)|(=)");
    auto begin{ std::sregex_iterator(
      input.begin(), 
      input.end(), 
      separating_regex) 
    };
    auto end{ std::sregex_iterator() };
    for( auto it{begin}; it!=end; it++ )
        result.push_back( it->str() );
    return result;
}

const std::string NativeParser::SyntaxDescription(
    "<command> "
    "[-<option-pack>] "
    "[--<option> [ = <option-parameter>]] "
    "[<command-parameter>]"); 

CommandLine NativeParser::parse(const std::string& input) const 
{
    auto tokens{ tokenize(input) };
    if ( !tokens.size() )
       throw InputException("NativeParser::parse()", "Received empty string"); 

    TokenHandler token_handler;
    for(auto it = tokens.begin(); it != tokens.end(); ++it)
    {
        token_handler.handle(*it);
    }
    return token_handler.fetch();
}

const std::string& NativeParser::getSyntaxDescription() const 
{
    return SyntaxDescription;
}
