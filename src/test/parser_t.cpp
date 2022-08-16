// Project......: Command Line Processor (clp)
// File.........: src/test/parser_t.cpp
// Author.......: elratmacfat
// Description..:
//
#define BOOST_TEST_MODULE parser_t

#include <boost/test/included/unit_test.hpp>

#include "elrat/clp/parser.hpp"

using namespace elrat;

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

struct clp_data_fixture {
    clp_data_fixture()
    : t({
        {"cmd","cmdp1","cmdp2"},
        {"o1","o1p1","o1p2"},
        {"o2","o2p1"},
        {"o3"}
      })
    {
        // done
    }

    ~clp_data_fixture() 
    {
        // done
    }

    clp::data t;
};

BOOST_AUTO_TEST_SUITE( data )

    BOOST_AUTO_TEST_CASE( access_existing_elements )
    {
        clp_data_fixture f;
        BOOST_CHECK( !f.t.empty() );
        
        BOOST_CHECK_EQUAL( f.t.cmd(), "cmd" );
        BOOST_CHECK_EQUAL( f.t.cmd_param_count(), 2 );
        BOOST_CHECK_EQUAL( f.t.cmd_param(1), "cmdp1" );
        BOOST_CHECK_EQUAL( f.t.cmd_param(2), "cmdp2" );
        
        BOOST_CHECK_EQUAL( f.t.opt_count(), 3 );
        
        BOOST_CHECK_EQUAL( f.t.opt_exists("o1"), 1 );
        BOOST_CHECK_EQUAL( f.t.opt_param_count(1), 2 );
        BOOST_CHECK_EQUAL( f.t.opt_param(1,1), "o1p1" );
        BOOST_CHECK_EQUAL( f.t.opt_param(1,2), "o1p2" );
        
        BOOST_CHECK_EQUAL( f.t.opt_exists("o2"), 2 );
        BOOST_CHECK_EQUAL( f.t.opt_param_count(2), 1 );
        BOOST_CHECK_EQUAL( f.t.opt_param(2,1), "o2p1" );
        
        BOOST_CHECK_EQUAL( f.t.opt_exists("o3"), 3 );
        BOOST_CHECK_EQUAL( f.t.opt_param_count(3), 0 );
    }

BOOST_AUTO_TEST_SUITE_END(); // data

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE( default_parser )

    BOOST_AUTO_TEST_CASE( tesets)
    {   

    }

BOOST_AUTO_TEST_SUITE_END(); // default_parser

