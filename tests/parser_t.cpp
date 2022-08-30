// Project......: Command Line Processor (clp)
// File.........: src/tests/parser_t.cpp
// Author.......: elratmacfat
// Description..:
//
#define BOOST_TEST_MODULE "Parser Test"
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <string>
#include <vector>

#include <elrat/clp.hpp>

using namespace elrat;

namespace valid_input
{
    const std::vector<std::string> commands_only {
         "x"
        ,"cmd"
        ,"_cmd"
        ,"_cmd-name"
        ,"cmd123"
    };
    
    const std::vector<std::string> commands_with_options {
         "x --option"
        ,"x --o1 --o2"
        ,"x -abc"
        ,"x -abc -def"
    };
}

namespace invalid_input
{
    const std::vector<std::string> commands_only {
         ""
        ,"1-command"
        ,"-cmd-name"
        ,"/cmd"
        ,"\\cmd"
        ,"?cmd"
        ,"cmd="
    };

    const std::vector<std::string> redundant_options {
         "x --a --a"
        ,"x -a --a"
        ,"x --a -a"
        ,"x -abc -abc"
        ,"x -abc -cba"
        ,"x -abc --b"
        ,"x --b -abc"
    };
}

void TryParsingCatchInvalidArgument(clp::Parser*, const std::string&);

BOOST_AUTO_TEST_SUITE( ParserTestSuite )

    clp::NativeParser t;

    BOOST_AUTO_TEST_CASE( CommandsOnly )
    {
        for( auto& input : valid_input::commands_only )
        {
            clp::CommandLine cmdline{ t.parse(input) };
            BOOST_CHECK_EQUAL( cmdline.getCommand(), input );
        }
    }

    BOOST_AUTO_TEST_CASE( InvalidCommands )
    {
        for( auto& input : invalid_input::commands_only )
        {
            TryParsingCatchInvalidArgument(&t, input);
        }
    }

    BOOST_AUTO_TEST_CASE( CommandsWithOptions )
    {
        for( auto& input : valid_input::commands_with_options )
        {
        }
    }

    BOOST_AUTO_TEST_CASE( CommandsWithRedundantOptions )
    {
        for( auto& input : invalid_input::redundant_options )
        {
            TryParsingCatchInvalidArgument(&t, input);
        }
    }

BOOST_AUTO_TEST_SUITE_END()

void TryParsingCatchInvalidArgument(clp::Parser* p, const std::string& input)
{
    bool thrown{false};
    try {
        p->parse(input);
    }
    catch( std::invalid_argument& exc ) {
        thrown = true;
    }
    BOOST_CHECK_MESSAGE( thrown, input );
}
