CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp CXX )

SET( CMAKE_CXX_STANDARD 17 )

SET( SourceDir "${CMAKE_CURRENT_SOURCE_DIR}/src" )
SET( HeaderDir "${CMAKE_CURRENT_SOURCE_DIR}/header" )
SET( IncludeDir "${CMAKE_CURRENT_SOURCE_DIR}/inc" )
SET( TestSourceDir "${CMAKE_CURRENT_SOURCE_DIR}/tests" )

SET( TestOutputDir "${CMAKE_CURRENT_BINARY_DIR}/tests" )
SET( ToolsOutputDir "${CMAKE_CURRENT_BINARY_DIR}/tools" )
SET( LibOutputDir "${CMAKE_CURRENT_BINARY_DIR}/lib" )

### L I B R A R Y #############################################################

SET( HeaderFiles
    "${HeaderDir}/elrat/clp.hpp" 
    "${HeaderDir}/elrat/clp/parser.hpp"
    "${HeaderDir}/elrat/clp/descriptors.hpp"
)

SET( IncludeFiles
    "${IncludeDir}/parser.hpp"
    "${IncludeDir}/regex.hpp"
)

SET( SourceFiles 
    "${SourceDir}/parser.cpp"
    "${SourceDir}/descriptors.cpp"
    "${SourceDir}/regex.cpp"
    "${SourceDir}/ostream.cpp"
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
    "${SourceDir}/tools/inspector.cpp" )
TARGET_LINK_LIBRARIES( inspector 
    PUBLIC clp )
SET_TARGET_PROPERTIES( inspector 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${ToolsOutputDir} )

### T E S T ###################################################################

FIND_PACKAGE( Boost 
    COMPONENTS unit_test_framework REQUIRED
)

IF ( Boost_FOUND )

	INCLUDE(CTest)
    
    ADD_EXECUTABLE( parser_test 
        "${TestSourceDir}/parser_t.cpp" )
    TARGET_LINK_LIBRARIES( parser_test
        PUBLIC clp 
        PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
    ADD_TEST( NAME parser 
        COMMAND parser_test )

    ADD_EXECUTABLE( descriptors_test
        "${TestSourceDir}/descriptors_t.cpp"
        "${TestSourceDir}/descriptors_t_input.cpp"
        "${TestSourceDir}/descriptors_t_util.cpp" )
    TARGET_INCLUDE_DIRECTORIES( descriptors_test
        PUBLIC ${TestSourceDir} )
    TARGET_LINK_LIBRARIES( descriptors_test
        PUBLIC clp
        PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
    ADD_TEST( NAME descriptors
        COMMAND descriptors_test )

    ADD_EXECUTABLE( clp_test
        "${TestSourceDir}/clp_test.cpp"
        "${TestSourceDir}/clp_test_input.cpp"
        "${TestSourceDir}/clp_test_util.cpp" )
    TARGET_INCLUDE_DIRECTORIES( clp_test
        PUBLIC ${TestSourceDir} )
    TARGET_LINK_LIBRARIES( clp_test
        PUBLIC clp
        PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
    ADD_TEST( NAME processor
        COMMAND clp_test )

    SET_TARGET_PROPERTIES( parser_test descriptors_test clp_test
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TestOutputDir} )

ELSE()
	MESSAGE( WARNING "Could not find 'Boost'. Skip compiling the test executable." )
ENDIF()

