CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp CXX )

SET( CMAKE_CXX_STANDARD 17 )

SET( SourceDir 
    "${CMAKE_CURRENT_SOURCE_DIR}/src" 
)

SET( HeaderDir 
    "${CMAKE_CURRENT_SOURCE_DIR}/header" 
)

SET( IncludeDir 
    "${CMAKE_CURRENT_SOURCE_DIR}/inc" 
)

###############################################################################
#
# Library
#

SET( HeaderFiles
    "${HeaderDir}/elrat/clp.hpp" 
)

SET( IncludeFiles
    "${IncludeDir}/parser.hpp"
    "${IncludeDir}/regex.hpp"
)

SET( SourceFiles 
    "${SourceDir}/parser.cpp"
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

###############################################################################
#
# Tools
#

ADD_EXECUTABLE( inspector 
    "${SourceDir}/tools/inspector.cpp"
)

TARGET_LINK_LIBRARIES( inspector 
    PUBLIC clp 
)

###############################################################################
#
# Test
#

FIND_PACKAGE( Boost 
    COMPONENTS unit_test_framework REQUIRED
)

IF ( Boost_FOUND )

	INCLUDE(CTest)
    
    ADD_EXECUTABLE( lowlevel-test 
        "${SourceDir}/tests/test1-lowlevel.cpp" 
    )
    
    TARGET_LINK_LIBRARIES( lowlevel-test
        PUBLIC clp 
        PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
    
    ADD_TEST( NAME lowlevel COMMAND lowlevel-test )

    ADD_EXECUTABLE( component-test src/tests/test2-component.cpp )
    TARGET_LINK_LIBRARIES( component-test
        PUBLIC clp
        PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
    ADD_TEST( NAME component COMMAND component-test )

ELSE()
	MESSAGE( WARNING "Could not find 'Boost'. Skip compiling the test executable." )
ENDIF()

