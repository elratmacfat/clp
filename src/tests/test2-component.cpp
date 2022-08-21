// Project......: Command Line Processor (clp)
// File.........: src/tests/test2-component.cpp
// Author.......: elratmacfat
// Description..: 
//
#define BOOST_TEST_MODULE "Test 2: "
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "elrat/clp/proc.hpp"

using namespace elrat::clp;

std::unique_ptr<proc> t;

BOOST_AUTO_TEST_SUITE( Processor_Setup )

    BOOST_AUTO_TEST_CASE( Initial_State )
    {
        t = std::make_unique<proc>();

        BOOST_CHECK(true);
    }

    BOOST_AUTO_TEST_CASE( Add_Commands )
    {
        BOOST_CHECK(true);
    }

    BOOST_AUTO_TEST_CASE( Context )
    {
        BOOST_CHECK(true);
    }
BOOST_AUTO_TEST_SUITE_END();



