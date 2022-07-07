# I don't know if it's actually *this* version that
# is required, but it is the one installed on my
# machine while I was creating this file.
CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp CXX)

INCLUDE(CheckIncludeFileCXX)

INCLUDE(CTest)

#
# Create the clp-library (shared)
#
ADD_LIBRARY(clp SHARED src/clp.cpp )
TARGET_INCLUDE_DIRECTORIES( clp	PUBLIC inc )

#
# Defines what happens 'make install' is invoked.
# (I'm not too familiar with the 'install' part. 
# This at least works for me on linux. The header
# will be installed to /usr/local/include and 
# the library goes into /usr/local/lib)
#
SET_TARGET_PROPERTIES( clp PROPERTIES PUBLIC_HEADER inc/elrat/clp.h )
INSTALL( TARGETS clp 
	LIBRARY DESTINATION lib 
	PUBLIC_HEADER DESTINATION include/elrat )

#
# Example application, linking against the library
#
ADD_EXECUTABLE( example src/example.cpp )
TARGET_LINK_LIBRARIES( example PUBLIC clp )
ADD_EXECUTABLE( inspect_cmdstr src/inspect_cmdstr.cpp )
TARGET_LINK_LIBRARIES( inspect_cmdstr PUBLIC clp )

#
# Try to find Boost and setup the target of the test executable.
#
FIND_PACKAGE( Boost )
IF ( Boost_FOUND )
	ADD_EXECUTABLE( clp_test src/test.cpp )
	TARGET_LINK_LIBRARIES( clp_test PUBLIC clp )
	TARGET_INCLUDE_DIRECTORIES( clp_test PUBLIC Boost_INCLUDE_DIRS )
	ADD_TEST( NAME clp_test COMMAND clp_test )
ELSE()
	MESSAGE( WARNING "Could not find 'Boost'. Skip compiling the test executable." )
ENDIF()

# Check if "elrat/testbench.h" is already installed on the system. 
#CHECK_INCLUDE_FILE_CXX( elrat/testbench.h include_found )
#IF ( include_found OR testbench_INCLUDE_DIRS )
#	ADD_EXECUTABLE( clp_test src/test.cpp )
#	TARGET_INCLUDE_DIRECTORIES( clp_test PUBLIC inc ${testbench_INCLUDE_DIRS} )
#	TARGET_LINK_LIBRARIES( clp_test PUBLIC clp )
#	ADD_TEST( NAME clp_test COMMAND clp_test )
#ELSE()
#	MESSAGE( WARNING "Required header file 'elrat/testbench.h' could not be found. Skip compiling test executable!" )
#ENDIF()

