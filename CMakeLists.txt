CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp CXX )

#
# Library
#

ADD_LIBRARY(clp SHARED src/clp.cpp )
TARGET_INCLUDE_DIRECTORIES( clp	PUBLIC inc )

#
# Install
#

SET_TARGET_PROPERTIES( clp PROPERTIES PUBLIC_HEADER inc/elrat/clp.h )
INSTALL( TARGETS clp 
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
	ADD_EXECUTABLE( unittest src/test.cpp )
	TARGET_LINK_LIBRARIES( unittest PUBLIC clp )
	TARGET_INCLUDE_DIRECTORIES( unittest PUBLIC Boost_INCLUDE_DIRS )
	ADD_TEST( NAME clp_unittest COMMAND unittest )
ELSE()
	MESSAGE( WARNING "Could not find 'Boost'. Skip compiling the test executable." )
ENDIF()

