// Project......: Command Line Processor (clp)
// File.........: src/tests/class-level-test/clt-data.cpp
// Author.......: elratmacfat
// Description..: Class-Level-Test
//                - data
//
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "elrat/clp/data.hpp"
#include <stdexcept>

using namespace elrat::clp;

BOOST_AUTO_TEST_SUITE( data )
   
    BOOST_AUTO_TEST_CASE( default_constructed )
    {
        data t;
        BOOST_CHECK( t.empty() );
        BOOST_CHECK_THROW( t.cmd(), std::out_of_range );
    }

    BOOST_AUTO_TEST_CASE( accessing_existing_elements )
    {
        data t({
	        {"cmd","cmdp1","cmdp2"},
	        {"o1","o1p1","o1p2"},
	        {"o2","o2p1"},
	        {"o3"}
	    });

        BOOST_CHECK( !t.empty() );
        
        BOOST_CHECK_EQUAL( t.cmd(), "cmd" );
        BOOST_CHECK_EQUAL( t.cmd_param_count(), 2 );
        BOOST_CHECK_EQUAL( t.cmd_param(1), "cmdp1" );
        BOOST_CHECK_EQUAL( t.cmd_param(2), "cmdp2" );
        
        BOOST_CHECK_EQUAL( t.opt_count(), 3 );
        
        BOOST_CHECK_EQUAL( t.opt_exists("o1"), 1 );
        BOOST_CHECK_EQUAL( t.opt_param_count(1), 2 );
        BOOST_CHECK_EQUAL( t.opt_param(1,1), "o1p1" );
        BOOST_CHECK_EQUAL( t.opt_param(1,2), "o1p2" );
        
        BOOST_CHECK_EQUAL( t.opt_exists("o2"), 2 );
        BOOST_CHECK_EQUAL( t.opt_param_count(2), 1 );
        BOOST_CHECK_EQUAL( t.opt_param(2,1), "o2p1" );
        
        BOOST_CHECK_EQUAL( t.opt_exists("o3"), 3 );
        BOOST_CHECK_EQUAL( t.opt_param_count(3), 0 );
    }

BOOST_AUTO_TEST_SUITE_END(); // data

