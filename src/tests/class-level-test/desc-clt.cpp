// Project......: Command Line Processor (clp)
// File.........: src/test/desc_t.cpp
// Author.......: elratmacfat
// Description..:
// 
// Testing validation (in the given order)
// - parameter type 
// - constraints
// - parameter 
// - option 
// - command 
//
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "elrat/clp/desc.hpp"

using namespace elrat;

BOOST_AUTO_TEST_SUITE( parameter_type_validation )
    

    // Natural number is a subset of whole numbers is a subset of real ...
    //
    //
    BOOST_AUTO_TEST_CASE( natural_number_c )
    {
        std::vector<std::string> candidates{
            "0",
            "15",
            "+1337",
            "0777",
            "0x0",
            "0xAB",
            "0xab"
        };
        for( auto& c : candidates ) {
            BOOST_CHECK_MESSAGE( clp::parameter_type::natural_number( c ), "candidate \"" << c << "\" failed." );
            BOOST_CHECK_MESSAGE( clp::parameter_type::whole_number( c ), "candidate \"" << c << "\" failed." );
        }
    }

    BOOST_AUTO_TEST_CASE( natural_number_failtest )
    {
        std::vector<std::string> candidates{
            "-1337", // negatives
            "1.0",   // real number
            "0xABH", //     "     hex
            ""       // empty
        };
        for( auto& c : candidates ) {
            BOOST_CHECK( !clp::parameter_type::natural_number( c ) );
        }
    }
    
    BOOST_AUTO_TEST_CASE( tc_whole_number )
    {
        std::vector<std::string> candidates{
            "-1",
            "-15"
        };
        for( auto& c : candidates ) {
            BOOST_CHECK( clp::parameter_type::whole_number( c ) );
            BOOST_CHECK( clp::parameter_type::real_number( c ) );
        }
    }

    BOOST_AUTO_TEST_CASE( whole_number_failtest )
    {
        std::vector<std::string> candidates{
            "-1.0",
            "+13.37", 
            ""
        };
        for( auto& c : candidates ) {
            BOOST_CHECK( !clp::parameter_type::whole_number( c ) );
        }
    }

    BOOST_AUTO_TEST_CASE( real_number )
    {
        std::vector<std::string> candidates{
            "133.7",
            "-.1",
            "+.025"
        };
        for( auto& c : candidates ) {
            BOOST_CHECK( clp::parameter_type::real_number( c ) );
        }
    }

    BOOST_AUTO_TEST_CASE( real_number_failtest )
    {
        std::vector<std::string> candidates{
            "-1.",
            ""
        };
        for( auto& c : candidates ) {
            BOOST_CHECK( !clp::parameter_type::real_number( c ) );
        }
    }
    
    BOOST_AUTO_TEST_CASE( name )
    {
        std::vector<std::string> candidates{
            "__name",
            "__command-name",
            "option-name",
            "opt_name",
            "opt-1337",
            "opt_13-37",
            "cmd-1337-opt"
        };
        for( auto& c : candidates ) {
            BOOST_CHECK_MESSAGE( clp::parameter_type::name(c), c );
        }

    }

    BOOST_AUTO_TEST_CASE( identifier )
    {
        std::vector<std::string> candidates{
            "__name",
            "__command_name",
            "option_name",
            "opt_name",
            "opt1337",
            "opt_1337",
            "cmd1337opt"
        };
        for( auto& c : candidates ) {
            BOOST_CHECK_MESSAGE( clp::parameter_type::identifier(c), c );
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

BOOST_AUTO_TEST_SUITE( parameter_validation )

    BOOST_AUTO_TEST_CASE( first )
    {

        BOOST_CHECK(true);
    }

BOOST_AUTO_TEST_SUITE_END() // parameter_validation


BOOST_AUTO_TEST_SUITE( option_validation )

    BOOST_AUTO_TEST_CASE( first )
    {

        BOOST_CHECK(true);
    }

BOOST_AUTO_TEST_SUITE_END() // option_validation

BOOST_AUTO_TEST_SUITE( command_validation )

    BOOST_AUTO_TEST_CASE( first )
    {
        BOOST_CHECK(true);
    }

BOOST_AUTO_TEST_SUITE_END() // command_validation
