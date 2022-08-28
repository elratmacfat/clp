#include <elrat/clp.hpp>

std::ostream& operator<<(std::ostream& os, const elrat::clp::CommandLine& cl)
{
    static auto print_parameters{
        [&os](bool indent, const std::vector<std::string>& vec)
        {
            for( int i{0}; i < vec.size(); i++ )
            {
                if (indent)
                    os << "  ";
                os << "-> Parameter #" << i;
                if (!indent)
                    os << "..";
                os << "....: \"" << vec[i] << "\"\n";
            }
        }
    };
    if (!cl.command.size()) 
        return os;
    
    os << "Command............: \""
        << cl.command
        << "\"\n";
    print_parameters( false, cl.parameters );
    
    for( auto& opt : cl.options )
    {        
        os << "-> Option" 
            << ".........: \""
            << opt.first 
            << "\"\n";
        print_parameters(true, opt.second);
    }
    return os;
}
