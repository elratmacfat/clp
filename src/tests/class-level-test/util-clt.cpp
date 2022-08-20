// Project......: Command Line Processor (clp)
// File.........: src/tests/class-level-test/util-clt.cpp
// Author.......: elratmacfat
// Description..:
//
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "elrat/clp/util.hpp"

BOOST_AUTO_TEST_SUITE( util_test_suite )
    BOOST_AUTO_TEST_CASE(xy)
    {   
        BOOST_CHECK(true);
    }
    /*
    using namespace elrat::clp;
    
    // util::regex_match
    // - takes a std::string_view
    // - checks if it matches the regex from start to finish.
    //
    BOOST_AUTO_TEST_CASE( regex_match_pt1 ) 
    {
        std::regex rgx("\\w+");
        std::vector<std::string_view> candidates {{
            "helloworld"    
            ,"hello123"
        }};

        for( auto c : candidates )
        {
            BOOST_CHECK( util::regex_match( c, rgx ) );
        }
    }

    BOOST_AUTO_TEST_CASE( regex_separate_pt1 )
    {
        const std::string src("abc");
        auto r1 = util::regex_separate( src, std::regex("abc") );
        BOOST_REQUIRE_EQUAL( r1.size(), 1 );
        BOOST_CHECK_EQUAL( r1[0], "abc" );
        auto r2 = util::regex_separate( src, std::regex("[\\w]+") );
        BOOST_REQUIRE_EQUAL( r2.size(), 1 );
        BOOST_CHECK_EQUAL( r2[0], "abc" );
    }
    BOOST_AUTO_TEST_CASE( regex_separate_pt2 ) 
    {
        const std::string source("abc def");
        auto results = util::regex_separate( source, std::regex("[^\\s]+"));
        BOOST_REQUIRE_EQUAL( results.size(), 2 );
        BOOST_CHECK_EQUAL( results[0], "abc" );
        BOOST_CHECK_EQUAL( results[1], "def" );
    }
    */
BOOST_AUTO_TEST_SUITE_END() // util



