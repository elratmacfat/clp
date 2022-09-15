#define BOOST_TEST_MODULE "Test"
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <elrat/clp.hpp>

#include "processor-test/util.hpp"
#include "processor-test/input.hpp"



BOOST_AUTO_TEST_SUITE( PROCESSOR )

    using namespace elrat::clp;

    Processor processor;

    auto cmdAdd = Add::Create();
    auto descAdd = initializeCommandDescriptor();

    BOOST_AUTO_TEST_CASE( ATTACH_DESCRIPTOR )
    {
        BOOST_CHECK_NO_THROW( processor.attach( descAdd ) ); 
    }
    
    BOOST_AUTO_TEST_CASE( ATTACH_DESCRIPTOR_FAILED )
    {
        BOOST_CHECK_THROW( processor.attach( descAdd ), 
            AlreadyInUseException ); 
    }

    BOOST_AUTO_TEST_CASE( ATTACH_COMMANDS )
    {
        BOOST_CHECK_NO_THROW( processor.attach( Add::GetName(), cmdAdd ) );
    }

    BOOST_AUTO_TEST_CASE( ATTACH_COMMANDS_FAILED )
    {
        BOOST_CHECK_THROW( processor.attach( Add::GetName(), cmdAdd ),
            AlreadyInUseException );
    }

    BOOST_AUTO_TEST_CASE( COMMAND_ADD )
    {

        BOOST_CHECK( !cmdAdd->isInitialized() );

        try { 
            processor.process("add 40.1 1.9");
        } catch( std::exception& e ) {
            BOOST_ERROR( e.what() );
        }

        BOOST_CHECK( cmdAdd->isInitialized() );
        BOOST_CHECK_EQUAL( cmdAdd->getValue(), 42.0 );

    }

    BOOST_AUTO_TEST_CASE( UNRECOGNIZED_COMMANDS )
    {
        const std::vector<std::string> undefined_commands {
            "xxx"
        };
        for( auto& input : undefined_commands )
        {
            BOOST_CHECK_THROW( processor.process(input), InvalidCommandException );
        }
    }

BOOST_AUTO_TEST_SUITE_END()


