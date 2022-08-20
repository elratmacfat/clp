// Project......: Command Line Processor (clp)
// File.........: src/tests/class-level-test/clt-default_parser.cpp
// Author.......: elratmacfat
// Description..: Class-Level-Test
//                - default_parser
// 
//
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "elrat/clp/parser.hpp"

using namespace elrat::clp;


// default_parser
//
// ->
// ->
//
BOOST_AUTO_TEST_SUITE( ts_default_parser )

    // auto => std::unique_ptr<parser>
    auto t{ parser::make<default_parser>() };
    using error = parser::error;
    error err;

    BOOST_AUTO_TEST_CASE( cmd_only )
    {   
        // auto => data
        auto data{ t->parse("cmd",err) };
        BOOST_REQUIRE( !data.empty() );
        BOOST_REQUIRE( !err );
        BOOST_CHECK_EQUAL( data.cmd(), "cmd" );
        BOOST_CHECK_EQUAL( data.opt_count(), 0 );
        BOOST_CHECK_EQUAL( data.cmd_param_count(), 0 );
    }
    
    BOOST_AUTO_TEST_CASE( cmd_with_param )
    {   
        auto data{ t->parse("cmd 12s3 .123 abc \"hello world\"", err) };
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
        auto data{ t->parse("cmd --a --opt-name --_abc", err) };
        BOOST_REQUIRE( data );
        BOOST_REQUIRE( !err );
        BOOST_CHECK_EQUAL( data.cmd_param_count(), 0 );
        BOOST_CHECK_EQUAL( data.opt_count(), 3 );
        BOOST_CHECK_EQUAL( data.opt(1), "a" );
        BOOST_CHECK_EQUAL( data.opt(2), "opt-name" );
        BOOST_CHECK_EQUAL( data.opt(3), "_abc" );
    }
    
    BOOST_AUTO_TEST_CASE( opt_with_param )
    {
        auto data{ t->parse("cmd --o1 = \"hello\" --cal=.177 x", err) };
        BOOST_REQUIRE( data );
        BOOST_REQUIRE( !err );
        BOOST_CHECK_EQUAL( data.cmd_param_count(), 1 );
        BOOST_CHECK_EQUAL( data.cmd_param(1), "x");
        BOOST_CHECK_EQUAL( data.opt_count(), 2 );
        BOOST_CHECK_EQUAL( data.opt(1), "o1" );
        BOOST_CHECK_EQUAL( data.opt_param_count(1), 1 );
        BOOST_CHECK_EQUAL( data.opt_param(1,1), "hello" );
        BOOST_CHECK_EQUAL( data.opt(2), "cal" );
        BOOST_CHECK_EQUAL( data.opt_param_count(2), 1 );
        BOOST_CHECK_EQUAL( data.opt_param(2,1), ".177" );
    }

    // Inventing test cases to find out what behaviour I would expect in those
    // situations.
    //
    BOOST_AUTO_TEST_CASE( syntax_error )
    {
        const std::vector<std::string> candidates {{
            "--cmd",
            "-cmd",
            "@cmd",
            "12cmd",
            "cmd=fail",
            "cmd -x=y"
        }};
        for( auto& c : candidates ) {
            auto data{ t->parse( c, err ) }; 
            BOOST_CHECK( !data );
            BOOST_CHECK( err() == error::code::syntax );
        }
    }   

    BOOST_AUTO_TEST_CASE( redundant_option )
    {
        const std::vector<std::string> candidates {{
            "cmd --x -x",
            "cmd -x --x"
            "cmd --x=y -x",
            "cmd -x --x=y",
        }};
        for( auto& c : candidates ) {
            auto data{ t->parse( c, err ) }; 
            BOOST_CHECK( !data );
            BOOST_CHECK( err() == error::code::redundant );
        }
    }

BOOST_AUTO_TEST_SUITE_END(); // default_parser

