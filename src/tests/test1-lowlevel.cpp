// Project......: Command Line Processor (clp)
// File.........: src/tests/test1-lowlevel.cpp
// Author.......: elratmacfat
// Description..:
//
#define BOOST_TEST_MODULE "Test 1: Low Level"
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "elrat/clp/desc.hpp"
#include "elrat/clp/parser.hpp"


// parameter descriptor -> parameter type 
//
//
BOOST_AUTO_TEST_SUITE( parameter_type_validation )
    
    using namespace elrat::clp::parameter_type;
    
    const std::vector<std::string> v_positive_integers{
        "0", "1", "89",             // regular natural numbers
        "+0", "+1337", "+9000",     // with preceeding plus-sign
        "0x0", "0xAb", "0XFf",      // hexadecimal
        "0777", "0134",             // octal 
    
    };
    
    const std::vector<std::string> v_negative_integers{
        "-0", "-1", "-89"
    };
    
    const std::vector<std::string> v_floating_points{
        "1.3", ".13", "13.0",
        "+1.3", "+.13", "+13.0",
        "-1.3", "-.13", "-13.0"
    };
    
    const std::vector<std::string> v_identifiers{
        "__name",
        "__command_name",
        "option_name",
        "opt_name",
        "opt1337",
        "opt_1337",
        "cmd1337opt"
    };
    
    const std::vector<std::string> v_names{
        "__name",
        "__command-name",
        "option-name",
        "opt_name",
        "opt-1337",
        "opt_13-37",
        "cmd-1337-opt"
    };
    
    // parameter_type::natural_number
    //
    // A natural number (set N*, zero is included)
    //
    //
    BOOST_AUTO_TEST_CASE( Natural_Number )
    {
        for( auto& c : v_positive_integers ) {
            BOOST_CHECK_MESSAGE( natural_number(c), c );
            BOOST_CHECK_MESSAGE( whole_number(c), c );
        }
    }
    
    BOOST_AUTO_TEST_CASE( Not_Natural_Number )
    {
        for( auto& c : v_negative_integers ) {
            BOOST_CHECK_MESSAGE( !natural_number(c), c );
        }
        for( auto& c : v_floating_points ) {
            BOOST_CHECK_MESSAGE( !natural_number(c), c );
        }
    }
    
    BOOST_AUTO_TEST_CASE( Whole_Number )
    {
        for( auto& c : v_positive_integers ) {
            BOOST_CHECK_MESSAGE( whole_number(c), c );
        }
        for( auto& c : v_negative_integers ) {
            BOOST_CHECK_MESSAGE( whole_number(c), c );
        }
    }
    
    BOOST_AUTO_TEST_CASE( Not_Whole_Number )
    {
        for( auto& c : v_floating_points ) {
            BOOST_CHECK_MESSAGE( !whole_number(c), c );
        }
    }
    
    BOOST_AUTO_TEST_CASE( Real_Number )
    {
        for( auto& c : v_floating_points ) {
            BOOST_CHECK_MESSAGE( real_number(c), c );
        }
    }
    
    BOOST_AUTO_TEST_CASE( Not_Real_Number )
    {
        std::vector<std::string> candidates{
            "-1.",
            ""
        };
        for( auto& c : candidates ) {
            BOOST_CHECK_MESSAGE( !real_number(c), c );
        }
    }
    
    BOOST_AUTO_TEST_CASE( Name )
    {
        for( auto& c : v_names ) {
            BOOST_CHECK_MESSAGE( name(c), c );
        }
    
    }
    
    BOOST_AUTO_TEST_CASE( Identifier )
    {
        std::vector<std::string> candidates{
            };
        for( auto& c : v_identifiers ) {
            BOOST_CHECK_MESSAGE( identifier(c), c );
        }
    
    }

BOOST_AUTO_TEST_SUITE_END(); // parameter_type_validation

BOOST_AUTO_TEST_SUITE( constraint_validation )

    using namespace elrat::clp;

    BOOST_AUTO_TEST_CASE( at_least_c )
    {
        auto p = constraint::at_least(10);
        BOOST_CHECK( !p->validate("9") );
        BOOST_CHECK( p->validate("10") );
        BOOST_CHECK( p->validate("11") );
    }

    BOOST_AUTO_TEST_CASE( at_most_c )
    {
        auto p = constraint::at_most(10);
        BOOST_CHECK( !p->validate("11"));
        BOOST_CHECK( p->validate("10"));
        BOOST_CHECK( p->validate("9"));
    }
    
    BOOST_AUTO_TEST_CASE( range_c )
    {
        auto p = constraint::in_range(5,10);
        BOOST_CHECK( !p->validate("4") );
        BOOST_CHECK( p->validate("5") );
        BOOST_CHECK( p->validate("10") );
        BOOST_CHECK( !p->validate("11") );

    }

    BOOST_AUTO_TEST_CASE( is_not_c )
    {
        auto p = constraint::is_not(5);
        BOOST_CHECK( p->validate("4") );
        BOOST_CHECK( !p->validate("5") );
        BOOST_CHECK( p->validate("6") );
    }

    BOOST_AUTO_TEST_CASE( in_c )
    {
        auto p = constraint::in(1,3,5);
        BOOST_CHECK( p->validate("1") );
        BOOST_CHECK( p->validate("3") );
        BOOST_CHECK( p->validate("5") );
        BOOST_CHECK( !p->validate("2") );
        BOOST_CHECK( !p->validate("4") );
        BOOST_CHECK( !p->validate("6") );

    }


BOOST_AUTO_TEST_SUITE_END() // constraint_validation



// native_parser
//
// ->
// ->
//
BOOST_AUTO_TEST_SUITE( ts_native_parser )
    
    using namespace elrat::clp;

    // auto => std::unique_ptr<parser>
    auto t{ parser::make<native_parser>() };
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

BOOST_AUTO_TEST_SUITE_END(); // native_parser

