#define BOOST_TEST_MODULE clp test module
#include <boost/test/included/unit_test.hpp>
#include <regex>
#include "elrat/clp/input.hpp"
#include "elrat/clp/parser.hpp"

using namespace elrat::clp;

struct InputFixture
{
    InputFixture() 
    : testee{ makeInput<Input>() }
    {
        testee->setCommand("cmd");
        testee->addCommandParameter("cmdparam");
        testee->addOption("opt1");
        testee->addOptionParameter("opt1","opt1param");
        testee->addOption("opt2");
    }
    ~InputFixture()
    {
    }
    
    std::shared_ptr<InputInterface> testee;
};

struct ParserFixture
{
    ParserFixture()
    : testee{ makeParser<Parser>( makeInput<Input>() ) }
    {
    }

    ~ParserFixture()
    {
    }

    std::shared_ptr<ParserInterface> testee;
};

BOOST_AUTO_TEST_SUITE( Input_Test )
    
    InputFixture f;

    BOOST_AUTO_TEST_CASE( verify_set_and_get )
    {
        BOOST_CHECK_EQUAL(f.testee->getCommand(), "cmd" );
        BOOST_CHECK_EQUAL(f.testee->getCommandParameterCount(), 1 );
        BOOST_CHECK_EQUAL(f.testee->getCommandParameter(0), "cmdparam" );
        
        BOOST_CHECK_EQUAL(f.testee->getOptionCount(), 2 );
        
        BOOST_CHECK( f.testee->optionExists("opt1") );
        BOOST_CHECK_EQUAL(f.testee->getOptionParameterCount("opt1"), 1 );
        BOOST_CHECK_EQUAL(f.testee->getOptionParameter("opt1",0), "opt1param");
        
        BOOST_CHECK( f.testee->optionExists("opt2") );
        BOOST_CHECK_EQUAL(f.testee->getOptionParameterCount("opt2"), 0 );
    }

    BOOST_AUTO_TEST_CASE( provoke_out_of_range_exceptions )
    {
        BOOST_CHECK_THROW(
            f.testee->getCommandParameter(1), 
            std::out_of_range );
        BOOST_CHECK_THROW( 
            f.testee->getOptionParameter("opt1",2),
            std::out_of_range);
        BOOST_CHECK_THROW(
            f.testee->getOptionParameter("xxx",0),
            std::out_of_range );
    }

BOOST_AUTO_TEST_SUITE_END(); // InputData_Test


BOOST_AUTO_TEST_SUITE( Parser_Test )

    BOOST_AUTO_TEST_CASE( default_constructed )
    {
        ParserFixture f;
        BOOST_CHECK( !f.testee->done() );
        BOOST_CHECK( !(*(f.testee->getResult())));
    }
    
    BOOST_AUTO_TEST_CASE( command_only )
    {
        ParserFixture f;    
        f.testee->parse("command");
        auto result{ f.testee->getResult() };
        BOOST_CHECK( f.testee->done() );
        BOOST_CHECK( *result );
        BOOST_CHECK_EQUAL( result->getCommand(), "command" );
        BOOST_CHECK_EQUAL( result->getCommandParameterCount(), 0 );
        BOOST_CHECK_EQUAL( result->getOptionCount(), 0 );
    }

    BOOST_AUTO_TEST_CASE( command_and_parameter )
    {       
        {
            ParserFixture f;
            f.testee->parse("command parameter");
            auto result{ f.testee->getResult() };
            BOOST_CHECK_EQUAL( result->getCommandParameterCount(), 1 );
            BOOST_CHECK_EQUAL( result->getCommandParameter(), "parameter");
        }
        {
            ParserFixture f;
            f.testee->parse("command 1234 -1234");
            auto result{ f.testee->getResult() };
            BOOST_CHECK_EQUAL( result->getCommandParameterCount(), 2 );
            BOOST_CHECK_EQUAL( result->getCommandParameter(0), "1234");
            BOOST_CHECK_EQUAL( result->getCommandParameter(1), "-1234");
        }
    }

    

    BOOST_AUTO_TEST_CASE( test )
    {
        ParserFixture f;   
        /*
        f.testee->parse("cmd cmdparam --opt1 opt1param --opt2");
        auto result{ f.testee->getResult() };
        BOOST_CHECK_EQUAL(result->getCommand(), "cmd" );
        BOOST_CHECK_EQUAL(result->getCommandParameterCount(), 1 );
        BOOST_CHECK_EQUAL(result->getCommandParameter(0), "cmdparam" );
        
        BOOST_CHECK_EQUAL(result->getOptionCount(), 2 );
        
        BOOST_CHECK( result->optionExists("opt1") );
        BOOST_CHECK_EQUAL(result->getOptionParameterCount("opt1"), 1 );
        BOOST_CHECK_EQUAL(result->getOptionParameter("opt1",0), "opt1param");
        
        BOOST_CHECK( result->optionExists("opt2") );
        BOOST_CHECK_EQUAL(result->getOptionParameterCount("opt2"), 0 );
        */
    }


BOOST_AUTO_TEST_SUITE_END(); // Parser_Test

