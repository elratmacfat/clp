// Project......: Command Line Processor (clp)
// File.........: elrat/clp.hpp
// Author.......: elratmacfat
// Description..: 
//
#ifndef ELRAT_CLP_HPP
#define ELRAT_CLP_HPP

#include <elrat/clp/errorhandling.hpp>
#include <elrat/clp/descriptors.hpp>
#include <elrat/clp/parser.hpp>

#include <map>

std::ostream& operator<<(std::ostream&,const elrat::clp::CommandLine&);

std::ostream& operator<<(std::ostream&,const elrat::clp::ValidationResult&);

namespace elrat {
namespace clp {
    
    class Command
    {
    public:
        virtual void execute(const CommandLine&) = 0;
    };

    using CommandPtr = std::shared_ptr<Command>;

    class Processor
    {
    public:
        Processor( std::shared_ptr<Parser> = std::make_shared<NativeParser>() );
        void setParser( std::shared_ptr<Parser> );
        void attachDescriptor(CommandDescriptorPtr);
        void attachCommand(const std::string&, CommandPtr);

        bool process(const std::string&) const;
        
    private:
        bool validateCommandLine(const CommandLine&) const;
        bool invokeCommand(const CommandLine&) const;
        
        std::shared_ptr<Parser> parser;
        std::vector<CommandDescriptorPtr> command_descriptors;
        std::map<std::string,std::vector<CommandPtr>> commands;

    };

} // clp
} // elrat

#endif

