#ifndef ELRAT_CLP_PARSER_INTERFACE_HPP
#define ELRAT_CLP_PARSER_INTERFACE_HPP

#include "elrat/clp/input_interface.hpp"

#include <memory>
#include <string>

namespace elrat {
namespace clp {

class ParserInterface
{
public:
    virtual ~ParserInterface()
    {
        // nothing
    }

    virtual void parse(const std::string& chunk) = 0;

    virtual bool done() const = 0;

    virtual std::shared_ptr<InputReaderInterface> getResult() = 0;
};

} // namespace clp
} // namespace elrat

#endif

