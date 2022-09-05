#ifndef DESCRIPTORS_T_UTIL_HPP
#define DESCRIPTORS_T_UTIL_HPP
#include <boost/test/unit_test.hpp>
#include <elrat/clp.hpp>

#include <string>
#include <vector>

template <class T> 
std::vector<std::string> convertRangeToStrings(
    T from, 
    T to, 
    T increment = static_cast<T>(1) );

std::string toString(const std::vector<std::string>&); 

namespace ParameterValidation
{
	void Check(
	    elrat::clp::TypeChecker, 
	    const std::vector<std::string>&);
	void FailCheck(
	    elrat::clp::TypeChecker, 
	    const std::vector<std::string>&);
	
	void Check(
	    elrat::clp::ConstraintPtr, 
	    const std::vector<std::string>&);
	void FailCheck(
	    elrat::clp::ConstraintPtr, 
	    const std::vector<std::string>&);
}    

namespace OptionValidation
{
	void Check( 
	    elrat::clp::OptionDescriptorPtr, 
        const std::string&,
	    const std::vector<std::string>&);
	void FailCheck(
	    elrat::clp::OptionDescriptorPtr, 
        const std::string&,
	    const std::vector<std::string>&);
	
    
    
    void CheckNoThrow( 
	    elrat::clp::OptionDescriptorPtr, 
        const std::string&,
	    const std::vector<std::string>&);

    template <class Exc>
    void CheckThrow(
	    elrat::clp::OptionDescriptorPtr, 
        const std::string&,
	    const std::vector<std::string>&);
}

namespace CommandValidation
{
	void Check( 
	    elrat::clp::CommandDescriptorPtr cmd_desc, 
	    const elrat::clp::CommandLine& input,
	    elrat::clp::ValidationResult expected );
}


// Template implementation
// vvvvvvvvvvvvvvvvvvvvvvv

template <class T>
std::vector<std::string> convertRangeToStrings(
    T from, 
    T to, 
    T increment )
{
    std::vector<std::string> result{};
    result.reserve( (to-from)/increment + 1 );
    while ( from <= to )
    {
        result.push_back(std::to_string(from));
        from+=increment;
    }
    return result;
}

template <class Exc>
void OptionValidation::CheckThrow(
	elrat::clp::OptionDescriptorPtr option, 
    const std::string& opt_name,
	const std::vector<std::string>& opt_parameters)
{
    try 
    {
        option->validate( opt_name, opt_parameters );
        BOOST_ERROR("No exception thrown");
    }
    catch( Exc& err )
    {
        BOOST_CHECK(true);
    }
    catch(std::exception& err)
    {
        BOOST_ERROR("Caught unexpected exception: " << err.what() );
    }
}

#endif

