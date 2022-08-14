#ifndef ELRAT_CLP_INPUT_HPP
#define ELRAT_CLP_INPUT_HPP

#include "elrat/clp/input_interface.hpp"

#include <map>
#include <memory>
#include <vector>

namespace elrat {
namespace clp {

class Input : public InputInterface
{
public:
    
    static std::shared_ptr<Input> MakeShared();

    // InputReaderInterface
    //

    operator bool() const;

    std::string_view getCommand() const;
    int getCommandParameterCount() const;

    std::string_view getCommandParameter(int index=0) const;

    bool optionExists(const std::string& opt) const;
    
    int getOptionCount() const;

    std::string_view getOption( int i ) const;

    int getOptionParameterCount(const std::string& opt) const;

    std::string_view getOptionParameter(const std::string& opt, 
                                        int index = 0 ) const;
    
    // InputInterface
    //

    void setCommand(const std::string&);

    void addCommandParameter(const std::string&);

    void addOption(const std::string&);

    void addOptionParameter(const std::string& opt,const std::string& param);

private:
    
    std::string _command;
    
    std::vector<std::string> _command_parameters;
    
    std::map<std::string,std::vector<std::string>> _options;
};

} // namespace clp 
} // namespace elrat

#endif // include guard



