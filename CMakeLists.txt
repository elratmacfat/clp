CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp CXX )

SET( CMAKE_CXX_STANDARD 17 )

#
# Library
#

SET( HeaderFiles
    inc/elrat/clp.hpp
    inc/elrat/clp/parser.hpp
)

SET( SourceFiles 
    src/clp.cpp
    src/parser.cpp
)

ADD_LIBRARY( clp SHARED ${PublicHeader} ${SourceFiles} )
TARGET_INCLUDE_DIRECTORIES( clp	PUBLIC inc )

#
# Tools
#
ADD_EXECUTABLE( inspector src/tools/inspector.cpp )
TARGET_LINK_LIBRARIES( inspector PUBLIC clp )

#
# Test
#
FIND_PACKAGE( Boost )
IF ( Boost_FOUND )

	INCLUDE(CTest)
    
    ADD_EXECUTABLE( parser_t src/test/parser_t.cpp )
    TARGET_LINK_LIBRARIES( parser_t PUBLIC clp )
	TARGET_INCLUDE_DIRECTORIES( parser_t PUBLIC Boost_INCLUDE_DIRS )

ELSE()
	MESSAGE( WARNING "Could not find 'Boost'. Skip compiling the test executable." )
ENDIF()

