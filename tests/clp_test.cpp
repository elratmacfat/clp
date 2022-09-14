#define BOOST_TEST_MODULE "Test"
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <elrat/clp.hpp>

#include "clp_test_util.hpp"
#include "clp_test_input.hpp"



BOOST_AUTO_TEST_SUITE( PROCESSOR )

    using namespace elrat::clp;

    Processor processor;

    DescriptorMapPtr descMapA{ initializeDescriptorMapA() };
    DescriptorMapPtr descMapB{ initializeDescriptorMapB() };

    auto cmdAdd = Add::Create();

    BOOST_AUTO_TEST_CASE( ATTACH_DESCRIPTOR_MAP )
    {
        BOOST_CHECK_NO_THROW( processor.attach( descMapA ) );
        BOOST_CHECK_NO_THROW( processor.attach( descMapB ) );
    }

    BOOST_AUTO_TEST_CASE( ATTACH_DESCRIPTOR_MAP_FAILED )
    {
        DescriptorMapPtr descMapX{ DescriptorMap::Create( descMapB->getName() ) };    
        BOOST_CHECK_THROW( processor.attach( descMapA ), InitializationException );
        BOOST_CHECK_THROW( processor.attach( descMapX ), InitializationException );
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

        BOOST_CHECK_NO_THROW( processor.process("add 40.1 1.9") );

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


