#define BOOST_TEST_MODULE clp test module
#include <boost/test/included/unit_test.hpp>

#include "elrat/clp/input.hpp"
#include "elrat/clp/parser.hpp"
using namespace elrat::clp;

struct InputFixture
{
    InputFixture() 
    {
        BOOST_TEST_MESSAGE("InputFixture setup");
    }
    ~InputFixture()
    {
        BOOST_TEST_MESSAGE("InputFixture teardown");
    }

    elrat::clp::Input input;
};



BOOST_AUTO_TEST_SUITE( Input_Test )
    BOOST_AUTO_TEST_CASE( default_constructed )
    {
        InputFixture f;
        BOOST_CHECK( !f.input );
        f.input.setCommand("Hello");
        BOOST_CHECK( f.input );
    }
    BOOST_AUTO_TEST_CASE( blah )
    {
        std::shared_ptr<InputInterface> i = Input::MakeShared();
        
        // "do_something a b --row param --force"
        i->setCommand("do_something");
        i->addOption("row");
        i->addOption("force");
        i->addCommandParameter("a");
        i->addCommandParameter("b");
        i->addOptionParameter("row","param");

        std::shared_ptr<InputReaderInterface> j = i;

        BOOST_CHECK_EQUAL( j->getCommand(), "do_something" );
        BOOST_CHECK_EQUAL( j->getCommandParameterCount(), 2 );
        BOOST_CHECK_EQUAL( j->getCommandParameter(0), "a" );
        BOOST_CHECK_EQUAL( j->getCommandParameter(1), "b" );
        
        BOOST_CHECK_EQUAL( j->getOptionCount(), 2 );
        
        BOOST_CHECK( j->optionExists("row") );
        BOOST_CHECK_EQUAL( j->getOptionParameterCount("row"), 1 );
        BOOST_CHECK_EQUAL( j->getOptionParameter("row",0), "param" );
        
        BOOST_CHECK( j->optionExists("force") );
        BOOST_CHECK_EQUAL( j->getOptionParameterCount("force"), 0 );




    }
BOOST_AUTO_TEST_SUITE_END(); // InputData_Test


BOOST_AUTO_TEST_SUITE( Parser_Test )




BOOST_AUTO_TEST_SUITE_END(); // Parser_Test

