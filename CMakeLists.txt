CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp CXX)

INCLUDE(CheckIncludeFileCXX)
INCLUDE(CTest)

ADD_LIBRARY(clp SHARED src/clp.cpp )
TARGET_INCLUDE_DIRECTORIES( clp	PUBLIC inc )
SET_TARGET_PROPERTIES( clp PROPERTIES PUBLIC_HEADER inc/elrat/clp.h )
INSTALL( TARGETS clp 
	LIBRARY DESTINATION lib 
	PUBLIC_HEADER DESTINATION include/elrat )

ADD_EXECUTABLE( example src/example.cpp )
TARGET_LINK_LIBRARIES( example PUBLIC clp )
ADD_EXECUTABLE( inspect_cmdstr src/inspect_cmdstr.cpp )
TARGET_LINK_LIBRARIES( inspect_cmdstr PUBLIC clp )

# Check if "elrat/testbench.h" is already installed on the system. 
CHECK_INCLUDE_FILE_CXX( elrat/testbench.h include_found )
IF ( include_found OR testbench_INCLUDE_DIRS )
	ADD_EXECUTABLE( clp_test src/test.cpp )
	TARGET_INCLUDE_DIRECTORIES( clp_test PUBLIC inc ${testbench_INCLUDE_DIRS} )
	TARGET_LINK_LIBRARIES( clp_test PUBLIC clp )
	ADD_TEST( NAME clp_test COMMAND clp_test )
ELSE()
	MESSAGE( WARNING "Required header file 'elrat/testbench.h' could not be found. Skip compiling test executable!" )
ENDIF()

