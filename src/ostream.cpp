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
                os << "..: \"" << vec[i] << "\"\n";
            }
        }
    };
    if (!cl.getCommand().size()) 
        return os;
    
    os << "Command............: \""
        << cl.getCommand()
        << "\"\n";
    print_parameters( false, cl.getCommandParameters() );
    
    int count = cl.getOptionCount();
    for( int i{0}; i < count; i++ )
    {        
        os << "-> Option" 
            << "..........: \""
            << cl.getOption(i)
            << "\"\n";
        print_parameters(true, cl.getOptionParameters(i));
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const elrat::clp::ValidationResult& v)
{
    using namespace elrat::clp;
    switch( v ) 
    {
    case ValidationResult::Valid:
        os << "Valid";
        break;
    case ValidationResult::InvalidCommand:
        os << "Invalid Command";
        break;
    case ValidationResult::InvalidParameter:
        os << "Invalid Parameter";
        break;
    case ValidationResult::MissingParameter:
        os << "Missing Parameter";
        break;
    case ValidationResult::InvalidOption:
        os << "Invalid Option";
        break;
    default:
        os << "Undefined Validation Result";
    }
    return os;
}
