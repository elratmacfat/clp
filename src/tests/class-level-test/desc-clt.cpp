// Project......: Command Line Processor (clp)
// File.........: src/test/desc_t.cpp
// Author.......: elratmacfat
// Description..:
//
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "elrat/clp/desc.hpp"

using namespace elrat;

BOOST_AUTO_TEST_SUITE( parametertype )
    

    // Natural number is a subset of whole numbers is a subset of real ...
    //
    //
    BOOST_AUTO_TEST_CASE( natural_number )
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
    
    BOOST_AUTO_TEST_CASE( whole_number )
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

BOOST_AUTO_TEST_SUITE_END(); // parameter_type

