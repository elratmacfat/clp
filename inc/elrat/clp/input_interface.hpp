#ifndef ELRAT_CLP_INPUT_INTERFACE_HPP
#define ELRAT_CLP_INPUT_INTERFACE_HPP

#include <string_view>

namespace elrat {
namespace clp {

class InputReaderInterface
{
public:

    virtual ~InputReaderInterface()
    {
        // nothing
    }

    virtual operator bool() const = 0;

    virtual std::string_view getCommand() const = 0;

    virtual int getCommandParameterCount() const = 0;

    virtual std::string_view getCommandParameter( 
        int index 
        ) const = 0;

    virtual int getOptionCount() const = 0;

    virtual std::string_view getOption(int i) const = 0;

    virtual bool optionExists( 
        const std::string& 
        ) const = 0;

    virtual int getOptionParameterCount( 
        const std::string& 
        ) const = 0;

    virtual std::string_view getOptionParameter( 
        const std::string&, 
        int index 
        ) const = 0;
};

class InputInterface : public InputReaderInterface
{
public:
    
    virtual ~InputInterface()
    {
        // nothing
    }

    virtual void setCommand( 
        const std::string& 
        ) = 0;

    virtual void addCommandParameter( 
        const std::string& 
        ) = 0;

    virtual void addOption(
        const std::string& 
        ) = 0;

    virtual void addOptionParameter(
        const std::string& opt,
        const std::string& param
        ) = 0;
};

} // namespace clp 
} // namespace elrat 

#endif

