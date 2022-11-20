#include "descriptors-unittest/utility.hpp"

using namespace elrat::clp;

std::string toString(const std::vector<std::string>& strings) 
{
    std::string result;
    std::stringstream ss;
    for( auto& p : strings )
        ss << p << " ";
    std::getline( ss, result );
    return result;
}

namespace ParameterValidation 
{
    
    namespace intern
    {
        void check( 
            bool expected, 
            TypeChecker typechecker,
            const std::vector<std::string>& candidates )
        {
            for( auto& c : candidates )
                BOOST_CHECK_MESSAGE( expected == typechecker(c), c );
        }

        void check(
            bool expected,
            ConstraintPtr constraint,
            const std::vector<std::string>& candidates )
        {
            for( auto& c : candidates )
                BOOST_CHECK_MESSAGE( expected == constraint->validate(c), c );
        }
    }
	
	void Check( 
	    TypeChecker typechecker, 
	    const std::vector<std::string>& candidates )
	{
        intern::check(true,typechecker,candidates);
	}
    
    void FailCheck( 
	    TypeChecker typechecker, 
	    const std::vector<std::string>& candidates )
	{
        intern::check(false,typechecker,candidates);
	}
	
	void Check( 
	    ConstraintPtr constraint, 
	    const std::vector<std::string>& candidates )
	{
        intern::check(true,constraint,candidates);
	}
		
	void FailCheck( 
	    ConstraintPtr constraint, 
	    const std::vector<std::string>& candidates )
	{
        intern::check(false,constraint,candidates);
	}


}

namespace OptionValidation
{
    namespace intern
    {
		void check(
		    bool expected,
		    OptionDescriptorPtr option, 
		    const std::string& opt_name,
		    const std::vector<std::string>& opt_parameters )
		{
		    try 
		    {
		        bool match = option->validate( opt_name, opt_parameters );
		        BOOST_CHECK_MESSAGE( match == expected, 
		            toString(opt_parameters) );
		    } 
		    catch( InputException& exc )
		    {
		        BOOST_ERROR( toString(opt_parameters) << ", " << exc.what() );
		    }
		    catch(...)
		    {
		        BOOST_ERROR( toString(opt_parameters) << ", " "Unknown exception");
		    }
		
		}
    }
	
	
	void Check( 
	    OptionDescriptorPtr option, 
	    const std::string& opt_name,
	    const std::vector<std::string>& opt_parameters )
	{
	    intern::check( true, option, opt_name, opt_parameters );
	}
	
	void FailCheck(
	    OptionDescriptorPtr option, 
	    const std::string& opt_name,
	    const std::vector<std::string>& opt_parameters )
	{
	    intern::check( false, option, opt_name, opt_parameters );    
	}

    void CheckNoThrow(
	    OptionDescriptorPtr option, 
	    const std::string& opt_name,
	    const std::vector<std::string>& opt_parameters )
    {
        try 
        {
            option->validate(opt_name,opt_parameters);
            BOOST_CHECK( true );
        }
        catch( InputException& err )
        {
            BOOST_ERROR( err.what() );
        }
        catch(...)
        {
            BOOST_ERROR("Unknown exception");
        }
    }
}

namespace CommandValidation
{
    namespace intern
    {
	    void check( 
		    bool expected,
		    CommandDescriptorPtr cmddesc, 
		    const CommandLine& cmdline )
		{
		    try 
		    {
		        bool match{cmddesc->validate(cmdline)};
		        BOOST_CHECK_MESSAGE( match == expected, "No message" );
		    }
		    catch( InputException& exc )
		    {
		        BOOST_ERROR( exc.what() );
		    }
		    catch( ... )
		    {
		        BOOST_ERROR("Unknown Exception");
		    }
		}

    }
		
	void Check(
	    CommandDescriptorPtr cmddesc, 
	    const CommandLine& cmdline )
	{
	    intern::check(true,cmddesc,cmdline);
	}
	
	void FailCheck(
	    CommandDescriptorPtr cmddesc, 
	    const CommandLine& cmdline )
	{
	    intern::check(false,cmddesc,cmdline);
	}
	
}

