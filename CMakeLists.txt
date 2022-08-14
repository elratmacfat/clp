CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp CXX )

SET( CMAKE_CXX_STANDARD 17 )

#
# Library
#

### Parser

ADD_LIBRARY( parser SHARED
    src/parser.cpp
    src/input.cpp
    inc/elrat/clp/input.hpp
    inc/elrat/clp/input_interface.hpp
    inc/elrat/clp/parser.hpp
    inc/elrat/clp/parser_interface.hpp )

TARGET_INCLUDE_DIRECTORIES( parser PUBLIC inc )

### CLP

ADD_LIBRARY( clp SHARED 
    src/clp.cpp )
TARGET_INCLUDE_DIRECTORIES( clp	PUBLIC inc )
TARGET_LINK_LIBRARIES( clp PUBLIC parser )

#
# Install
#
SET_TARGET_PROPERTIES( parser PROPERTIES
    PUBLIC_HEADER inc/elrat/clp/input.hpp
    PUBLIC_HEADER inc/elrat/clp/input_interface.hpp
    PUBLIC_HEADER inc/elrat/clp/parser.hpp
    PUBLIC_HEADER inc/elrat/clp/parser_interface.hpp )

SET_TARGET_PROPERTIES( clp PROPERTIES 
    PUBLIC_HEADER 
        inc/elrat/clp.hpp )

INSTALL( TARGETS parser clp 
	LIBRARY DESTINATION lib 
	PUBLIC_HEADER DESTINATION include/elrat )

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
    TARGET_LINK_LIBRARIES( parsertest PUBLIC parser )
	TARGET_INCLUDE_DIRECTORIES( parsertest PUBLIC Boost_INCLUDE_DIRS )

ELSE()
	MESSAGE( WARNING "Could not find 'Boost'. Skip compiling the test executable." )
ENDIF()

