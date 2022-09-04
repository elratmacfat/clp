#include <elrat/clp.hpp>

#include <sstream>

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
    case ValidationResult::Match:
        os << "Match.";
        break;
    case ValidationResult::NoMatch:
        os << "No match.";
        break;
    case ValidationResult::InvalidCommand:
        os << "Unrecognized command.";
        break;
    case ValidationResult::InvalidOption:
        os << "Unrecognized option.";
        break;
    case ValidationResult::InvalidParameterType:
        os << "Invalid parameter type.";
        break;
    case ValidationResult::InvalidParameterValue:
        os << "Invalid parameter value.";
        break;
    case ValidationResult::MissingParameters:
        os << "One or more parameters are missing.";
        break;
    case ValidationResult::TooManyParameters:
        os << "Too many parameters assigned.";
        break;
    default:
        os << "Undefined validation result.";
    }
    return os;
}

std::string toString(const elrat::clp::ValidationResult& v)
{
    std::stringstream ss;
    std::string result;
    ss << v;
    std::getline(ss,result);
    return result;
}
