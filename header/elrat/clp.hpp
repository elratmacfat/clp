#ifndef ELRAT_CLP_HPP
#define ELRAT_CLP_HPP

#include <elrat/clp/commands.hpp>
#include <elrat/clp/descriptors.hpp>
#include <elrat/clp/errorhandling.hpp>
#include <elrat/clp/parser.hpp>

#include <map>

std::ostream& operator<<(std::ostream&,const elrat::clp::CommandLine&);

namespace elrat {
namespace clp {

    class Processor
    {
    public:
        Processor( std::shared_ptr<Parser> = std::make_shared<NativeParser>() );
        
        void attach(DescriptorMapPtr);
       
        bool process(const std::string&) const;
        
    private:
        
        std::shared_ptr<Parser>         parser;
        std::vector<DescriptorMapPtr>   descriptor_maps;
        Commands                        commands;
    };

} // clp
} // elrat

#endif

