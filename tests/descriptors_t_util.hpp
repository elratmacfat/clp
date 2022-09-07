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

    template <class EXCEPTION>
    void CheckThrow(
	    elrat::clp::OptionDescriptorPtr, 
        const std::string&,
	    const std::vector<std::string>&);
}

namespace CommandValidation
{
	void Check( 
	    elrat::clp::CommandDescriptorPtr cmd_desc, 
	    const elrat::clp::CommandLine& input );

    void FailCheck(
	    elrat::clp::CommandDescriptorPtr cmd_desc, 
	    const elrat::clp::CommandLine& input );

    template <class EXCEPTION>
    void CheckThrow(
	    elrat::clp::CommandDescriptorPtr cmd_desc, 
	    const elrat::clp::CommandLine& input );
}

namespace intern
{
    template <class EXCEPTION, class MEMBER_FUNCTION, class OBJECT, class...ARGS>
    void CheckThrow( MEMBER_FUNCTION function, OBJECT object, ARGS...args);
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

template <class EXCEPTION>
void OptionValidation::CheckThrow(
	elrat::clp::OptionDescriptorPtr option, 
    const std::string& opt_name,
	const std::vector<std::string>& opt_parameters)
{   
    intern::CheckThrow<EXCEPTION>(
        &elrat::clp::OptionDescriptor::validate,
        *option,
        opt_name,
        opt_parameters );
}

template <class EXCEPTION>
void CommandValidation::CheckThrow(
    elrat::clp::CommandDescriptorPtr cmd_desc, 
    const elrat::clp::CommandLine& input )
{
    intern::CheckThrow<EXCEPTION>(
        &elrat::clp::CommandDescriptor::validate,
        *cmd_desc,
        input );
}

template <class EXCEPTION, class MEMBER_FUNCTION, class OBJECT, class...ARGS>
void intern::CheckThrow( MEMBER_FUNCTION function, OBJECT object, ARGS...args)
{
    try 
    {
        (object.*function)(args...);
        BOOST_ERROR("No exception thrown");
    }
    catch( EXCEPTION& exception )
    {
        BOOST_CHECK(true);
    }
    catch( std::exception& exception )
    {
        BOOST_ERROR("Wrong exception: " << exception.what() );
    }
    catch( ... )
    {
        BOOST_ERROR("Wrong unknown exception.");
    }
}


#endif

