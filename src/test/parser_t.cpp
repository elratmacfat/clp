// Project......: Command Line Processor (clp)
// File.........: src/test/parser_t.cpp
// Author.......: elratmacfat
// Description..:
//
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "elrat/clp/parser.hpp"

#include <stdexcept>

using namespace elrat;

// clp::data
//
// -> initialization verification
// -> accessing non-existent elements
//
BOOST_AUTO_TEST_SUITE( data )
    
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

    BOOST_AUTO_TEST_CASE( default_constructed )
    {
        clp::data t;
        BOOST_CHECK( t.empty() );
        BOOST_CHECK_THROW( t.cmd(), std::out_of_range );
    }

    BOOST_AUTO_TEST_CASE( accessing_existing_elements )
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

// clp::default_parser
//
// ->
// ->
//
BOOST_AUTO_TEST_SUITE( default_parser )

    // auto => std::unique_ptr<clp::parser>
    auto parser{ clp::parser::make<clp::default_parser>() };
    clp::parser::error err;

    BOOST_AUTO_TEST_CASE( cmd_only )
    {   
        // auto => clp::data
        auto data{ parser->parse("cmd",err) };
        BOOST_REQUIRE( !data.empty() );
        BOOST_REQUIRE( !err );
        BOOST_CHECK_EQUAL( data.cmd(), "cmd" );
        BOOST_CHECK_EQUAL( data.opt_count(), 0 );
        BOOST_CHECK_EQUAL( data.cmd_param_count(), 0 );
    }
    
    BOOST_AUTO_TEST_CASE( cmd_with_param )
    {   
        auto data{ parser->parse("cmd 12s3 .123 abc \"hello world\"", err) };
        BOOST_REQUIRE( data );
        BOOST_REQUIRE( !err );
        BOOST_CHECK_EQUAL( data.opt_count(), 0 );
        BOOST_CHECK_EQUAL( data.cmd_param_count(), 4 );
        BOOST_CHECK_EQUAL( data.cmd_param(1), "12s3" );
        BOOST_CHECK_EQUAL( data.cmd_param(2), ".123" );
        BOOST_CHECK_EQUAL( data.cmd_param(3), "abc" );
        BOOST_CHECK_EQUAL( data.cmd_param(4), "hello world" );
    }

    BOOST_AUTO_TEST_CASE( cmd_with_opt )
    {
        auto data{ parser->parse("cmd --a --opt-name --_abc", err) };
        BOOST_REQUIRE( data );
        BOOST_REQUIRE( !err );
        BOOST_CHECK_EQUAL( data.cmd_param_count(), 0 );
        BOOST_CHECK_EQUAL( data.opt_count(), 3 );
        BOOST_CHECK_EQUAL( data.opt(1), "a" );
        BOOST_CHECK_EQUAL( data.opt(2), "opt-name" );
        BOOST_CHECK_EQUAL( data.opt(3), "_abc" );
    }

BOOST_AUTO_TEST_SUITE_END(); // default_parser

