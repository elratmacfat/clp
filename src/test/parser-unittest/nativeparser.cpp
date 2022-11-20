#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <string>
#include <vector>

#include "elrat/clp/nativeparser.hpp"
#include "elrat/clp/errorhandling.hpp"

using namespace elrat;

namespace valid
{
    const std::vector<std::string> commands {
         "x"
        ,"cmd"
        ,"_cmd"
        ,"_cmd-name"
        ,"cmd123"
    };
    
    const std::vector<std::string> options {
         "x --option"
        ,"x --o1 --o2"
        ,"x -abc"
        ,"x -abc -def"
    };
    const std::vector<std::string> one_parameter {
         "x a"
        ,"x \"a b c\""
        ,"x 123"
        ,"x a-b-c"
        ,"x a -opt --option"
        ,"x -.01f"
        ,"x +1.5"
    };
    const std::vector<std::string> two_parameters {
         "x a b"
        ,"x \"abc\"\"d e f\""
        ,"x \"a b c\" \"abc\""
        ,"x a --option b"
        ,"x a --option = z b"
    };
}

namespace invalid
{
    const std::vector<std::string> commands {
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

void TryParsingCatchInputException(clp::Parser*, const std::string&);
void TryParsingCheckNoThrow(clp::Parser*, const std::string&);

BOOST_AUTO_TEST_SUITE( ParserTestSuite )

    clp::NativeParser t;

    BOOST_AUTO_TEST_CASE( CommandsOnly )
    {
        for( auto& input : valid::commands )
        {
            clp::CommandLine cmdline{ t.parse(input) };
            BOOST_CHECK_EQUAL( cmdline.getCommand(), input );
        }
    }

    BOOST_AUTO_TEST_CASE( InvalidCommands )
    {
        for( auto& input : invalid::commands )
        {
            TryParsingCatchInputException(&t, input);
        }
    }

    BOOST_AUTO_TEST_CASE( CommandsWithOptions )
    {
        for( auto& input : valid::options )
        {
            TryParsingCheckNoThrow(&t, input); 
        }
    }

    BOOST_AUTO_TEST_CASE( CommandsWithRedundantOptions )
    {
        for( auto& input : invalid::redundant_options )
        {
            TryParsingCatchInputException(&t, input);
        }
    }

    BOOST_AUTO_TEST_CASE( CommandsWithOneParameter )
    {
        for( auto& input : valid::one_parameter )
        {
            auto cl{ t.parse(input) };
            BOOST_CHECK_MESSAGE( cl.getCommandParameters().size() == 1, input );
        }
    }

    BOOST_AUTO_TEST_CASE( CommandsWithTwoParameters )
    {
        for( auto& input : valid::two_parameters )
        {
            auto cl{ t.parse(input) };
            BOOST_CHECK_MESSAGE( cl.getCommandParameters().size() == 2, input );
        }
    }


BOOST_AUTO_TEST_SUITE_END()

void TryParsingCatchInputException(clp::Parser* p, const std::string& input)
{
    bool thrown{false};
    try {
        p->parse(input);
    }
    catch( elrat::clp::InputException& exc ) {
        thrown = true;
    }
    BOOST_CHECK_MESSAGE( thrown, input );
}

void TryParsingCheckNoThrow(clp::Parser* p, const std::string& input)
{
    bool thrown{true};
    std::string msg;
    try {
        p->parse(input);
        thrown = false;
    }
    catch(std::exception& exc ) {
        msg = std::string(exc.what());
    }
    catch(...) {
        msg = std::string("Caught non-std::exception");
    }
    BOOST_CHECK_MESSAGE( !thrown, "Candidate: [" << input << "], " << msg );
}
