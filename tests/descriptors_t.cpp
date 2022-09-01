// Project......: Command Line Processor (clp)
// File.........: src/tests/descriptors_t.cpp
// Author.......: elratmacfat
// Description..: 
//
#define BOOST_TEST_MODULE "CLP - Descriptors"
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <elrat/clp.hpp>

using namespace elrat::clp;

CommandDescriptorPtr cmd_ls = 
    Command("ls", "list items...", {
        Parameter("dir","present in the specified directory.",Optional,Path)
    },{
        Option("a","include hidden items"),
        Option("l","print additional details")
    });

CommandDescriptorPtr cmd_cp = 
    Command("cp", "copy files", {
        Parameter("src","source file",Mandatory,Path),
        Parameter("dest", "destination file or directory",Mandatory,Path)
    });

CommandDescriptors cmds{ 
};

BOOST_AUTO_TEST_SUITE( DescritorsTestSuite )
    BOOST_AUTO_TEST_CASE( Context )
    {
        BOOST_CHECK(true);
    }
BOOST_AUTO_TEST_SUITE_END();

