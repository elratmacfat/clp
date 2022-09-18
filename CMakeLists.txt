CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp CXX )

SET( CMAKE_CXX_STANDARD 17 )

SET( SourceDir "${CMAKE_CURRENT_SOURCE_DIR}/source" )
SET( HeaderDir "${CMAKE_CURRENT_SOURCE_DIR}/header" )
SET( IncludeDir "${CMAKE_CURRENT_SOURCE_DIR}/include" )
SET( TestSourceDir "${CMAKE_CURRENT_SOURCE_DIR}/tests" )
SET( ToolsSourceDir "${CMAKE_CURRENT_SOURCE_DIR}/tools" )
SET( ExamplesSourceDir "${CMAKE_CURRENT_SOURCE_DIR}/examples" )

SET( TestOutputDir "${CMAKE_CURRENT_BINARY_DIR}/tests" )
SET( ToolsOutputDir "${CMAKE_CURRENT_BINARY_DIR}/tools" )
SET( ExamplesOutputDir "${CMAKE_CURRENT_BINARY_DIR}/examples" )
SET( LibOutputDir "${CMAKE_CURRENT_BINARY_DIR}/lib" )

### L I B R A R Y #############################################################

SET( HeaderFiles
    "${HeaderDir}/elrat/clp.hpp" 
    "${HeaderDir}/elrat/clp/commandline.hpp"
    "${HeaderDir}/elrat/clp/commandmap.hpp"
    "${HeaderDir}/elrat/clp/convert.hpp"
    "${HeaderDir}/elrat/clp/descriptors.hpp"
    "${HeaderDir}/elrat/clp/errorhandling.hpp"
    "${HeaderDir}/elrat/clp/parser.hpp"
)

SET( IncludeFiles
    "${IncludeDir}/builtin.hpp"
    "${IncludeDir}/commandwrapper.hpp"
    "${IncludeDir}/parser.hpp"
    "${IncludeDir}/regex.hpp"
)

SET( SourceFiles 
    "${SourceDir}/builtin.cpp"
    "${SourceDir}/commandline.cpp"
    "${SourceDir}/commandmap.cpp"
    "${SourceDir}/commandwrapper.cpp"
    "${SourceDir}/convert.cpp"
    "${SourceDir}/descriptors.cpp"
    "${SourceDir}/errorhandling.cpp"
    "${SourceDir}/ostream.cpp"
    "${SourceDir}/parser.cpp"
    "${SourceDir}/processor.cpp"
    "${SourceDir}/regex.cpp"
)

ADD_LIBRARY( clp SHARED 
    ${HeaderFiles} 
    ${IncludeFiles} 
    ${SourceFiles}
)

TARGET_INCLUDE_DIRECTORIES( clp	
    PUBLIC ${HeaderDir}
    PRIVATE ${IncludeDir}
)

SET_TARGET_PROPERTIES( clp
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LibOutputDir}
    ARCHIVE_OUTPUT_DIRECTORY ${LibOutputDir} )

### T O O L S #################################################################

ADD_EXECUTABLE( inspector 
    "${ToolsSourceDir}/inspector.cpp" )
TARGET_LINK_LIBRARIES( inspector 
    PUBLIC clp )
SET_TARGET_PROPERTIES( inspector 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${ToolsOutputDir} )

### E X A M P L E S ###########################################################

ADD_EXECUTABLE( helloworld-1 "${ExamplesSourceDir}/helloworld-1.cpp" )
ADD_EXECUTABLE( helloworld-2 "${ExamplesSourceDir}/helloworld-2.cpp" )
ADD_EXECUTABLE( builtin-commands "${ExamplesSourceDir}/builtin-commands.cpp" )
TARGET_LINK_LIBRARIES( helloworld-1 PUBLIC clp )
TARGET_LINK_LIBRARIES( helloworld-2 PUBLIC clp )
TARGET_LINK_LIBRARIES( builtin-commands PUBLIC clp )

SET_TARGET_PROPERTIES( 
    helloworld-1 
    helloworld-2 
    builtin-commands
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${ExamplesOutputDir} )

### T E S T ###################################################################

FIND_PACKAGE( Boost 
    COMPONENTS unit_test_framework REQUIRED
)

IF ( Boost_FOUND )

    INCLUDE(CTest)
    
    ADD_EXECUTABLE( parser_test 
        "${TestSourceDir}/parser-test/test.cpp" )
    TARGET_LINK_LIBRARIES( parser_test
        PUBLIC clp 
        PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
    ADD_TEST( NAME parser 
        COMMAND parser_test )

    ADD_EXECUTABLE( descriptors_test
        "${TestSourceDir}/descriptors-test/test.cpp"
        "${TestSourceDir}/descriptors-test/input.cpp"
        "${TestSourceDir}/descriptors-test/util.cpp" )
    TARGET_INCLUDE_DIRECTORIES( descriptors_test
        PUBLIC ${TestSourceDir} )
    TARGET_LINK_LIBRARIES( descriptors_test
        PUBLIC clp
        PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
    ADD_TEST( NAME descriptors
        COMMAND descriptors_test )

    ADD_EXECUTABLE( processor_test
        "${TestSourceDir}/processor-test/test.cpp"
        "${TestSourceDir}/processor-test/input.cpp"
        "${TestSourceDir}/processor-test/util.cpp" )
    TARGET_INCLUDE_DIRECTORIES( processor_test
        PUBLIC ${TestSourceDir} )
    TARGET_LINK_LIBRARIES( processor_test
        PUBLIC clp
        PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
    ADD_TEST( NAME processor
        COMMAND processor_test )

    SET_TARGET_PROPERTIES( parser_test descriptors_test processor_test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TestOutputDir} )

ELSE()
	MESSAGE( WARNING "Could not find 'Boost'. Skip compiling the test executable." )
ENDIF()

