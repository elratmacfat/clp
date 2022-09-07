#define BOOST_TEST_MODULE "Test"
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <elrat/clp.hpp>

#include "clp_test_util.hpp"
#include "clp_test_input.hpp"



BOOST_AUTO_TEST_SUITE( NO_CONTEXT )

    using namespace elrat::clp;

    Processor processor;

    DescriptorMapPtr basic_cmd_1 = DescriptorMap::Create("basic commands");
    DescriptorMapPtr basic_cmd_2 = DescriptorMap::Create("basic commands");
    DescriptorMapPtr special_cmd = DescriptorMap::Create("special commands");

    BOOST_AUTO_TEST_CASE( ATTACH_DESCRIPTOR_MAP )
    {
        BOOST_CHECK_NO_THROW( processor.attach( basic_cmd_1 ) );
        BOOST_CHECK_NO_THROW( processor.attach( special_cmd ) );

        BOOST_CHECK_THROW( processor.attach( basic_cmd_1), 
            InitializationException );
        BOOST_CHECK_THROW( processor.attach( basic_cmd_2), 
            InitializationException );
    }

BOOST_AUTO_TEST_SUITE_END()



