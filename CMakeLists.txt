CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp CXX )

SET( CMAKE_CXX_STANDARD 17 )

#
# Library
#

SET( PublicHeader
    inc/elrat/clp.hpp
    inc/elrat/clp/parser_interface.hpp 
    inc/elrat/clp/parser.hpp
    inc/elrat/clp/input_interface.hpp
    inc/elrat/clp/input.hpp
)

SET( IncludeFiles 
    inc/elrat/clp/common.hpp
)

SET( SourceFiles 
    src/clp.cpp
    src/common.cpp
    src/parser.cpp
    src/input.cpp
)

ADD_LIBRARY( clp SHARED 
    ${PublicHeader}
    ${IncludeFiles}
    ${SourceFiles} )

TARGET_INCLUDE_DIRECTORIES( clp	PUBLIC inc )

#
# Example
#

ADD_EXECUTABLE( example src/example.cpp )
TARGET_LINK_LIBRARIES( example PUBLIC clp )

ADD_EXECUTABLE( inspect_cmdstr src/inspect_cmdstr.cpp )
TARGET_LINK_LIBRARIES( inspect_cmdstr PUBLIC clp )

#
# Test
#

FIND_PACKAGE( Boost )
IF ( Boost_FOUND )

	INCLUDE(CTest)
    
    ADD_EXECUTABLE( oldtest src/test/old_test.cpp )
	TARGET_LINK_LIBRARIES( oldtest PUBLIC clp )
	TARGET_INCLUDE_DIRECTORIES( oldtest PUBLIC Boost_INCLUDE_DIRS )
	ADD_TEST( NAME oldtest COMMAND unittest )
    
    ADD_EXECUTABLE( parsertest src/test/parser_test.cpp )
    TARGET_LINK_LIBRARIES( parsertest PUBLIC clp )
	TARGET_INCLUDE_DIRECTORIES( parsertest PUBLIC Boost_INCLUDE_DIRS )

ELSE()
	MESSAGE( WARNING "Could not find 'Boost'. Skip compiling the test executable." )
ENDIF()

