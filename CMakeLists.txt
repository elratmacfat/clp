CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp 
	VERSION 0.1.0 
	DESCRIPTION "Command Line Processor" 
	LANGUAGES CXX 
)

SET( CMAKE_CXX_STANDARD 17 )

FIND_PACKAGE( Boost 
	COMPONENTS unit_test_framework 
)

IF( ${Boost_FOUND} )
	INCLUDE(CTest)
ENDIF()

#
# Library
#
SET( public_header 
	header/elrat/clp/clp.hpp
	header/elrat/clp/command.hpp
	header/elrat/clp/commandline.hpp
	header/elrat/clp/commandmap.hpp
	header/elrat/clp/convert.hpp
	header/elrat/clp/descriptors.hpp
	header/elrat/clp/errorhandling.hpp
	header/elrat/clp/nativeparser.hpp
	header/elrat/clp/parser.hpp
	header/elrat/clp/parserwrapper.hpp
	header/elrat/clp/processor.hpp
)

ADD_LIBRARY(clp
	source/common/commandline.cpp
	source/common/errorhandling.cpp
	source/common/regex.cpp
	source/descriptors/descriptors.cpp
	source/parser/parser.cpp
	source/parser/parserwrapper.cpp
	source/parser/nativeparser/nativeparser.cpp
	source/parser/nativeparser/tokenhandler.cpp
	source/processor/builtin.cpp
	source/processor/command.cpp
	source/processor/commandmap.cpp
	source/processor/commandwrapper.cpp
	source/processor/processor.cpp
)

TARGET_INCLUDE_DIRECTORIES( clp
	PUBLIC header
	PRIVATE source
)

SET_TARGET_PROPERTIES( clp PROPERTIES 
	VERSION ${PROJECT_VERSION}
	PUBLIC_HEADER "${public_header}"
	LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib 
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib 

)

#
# Sample Applications
#
ADD_EXECUTABLE( hello-world-1 examples/helloworld-1.cpp )
TARGET_LINK_LIBRARIES( hello-world-1 PRIVATE clp )

ADD_EXECUTABLE( hello-world-2 examples/helloworld-2.cpp )
TARGET_LINK_LIBRARIES( hello-world-2 PRIVATE clp )

ADD_EXECUTABLE( inspector examples/inspector.cpp )
TARGET_LINK_LIBRARIES( inspector PRIVATE clp )

ADD_EXECUTABLE( built-in-commands examples/builtin-commands.cpp )
TARGET_LINK_LIBRARIES( built-in-commands PRIVATE clp )

SET_TARGET_PROPERTIES( 
	hello-world-1 
	hello-world-2 
	inspector 
	built-in-commands
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)
	

#
# Unit-Tests
#
IF ( Boost_FOUND )
	INCLUDE(CTest)

	ADD_EXECUTABLE( unittest
		test/unittest.cpp 
		test/parser-unittest/nativeparser.cpp
		test/descriptors-unittest/testsuites.cpp
		test/descriptors-unittest/inputdata.cpp
		test/descriptors-unittest/utility.cpp
		test/processor-unittest/testsuites.cpp
		test/processor-unittest/inputdata.cpp
		test/processor-unittest/utility.cpp
	)

	TARGET_INCLUDE_DIRECTORIES( unittest
		PRIVATE test
	)

	TARGET_LINK_LIBRARIES( unittest
		PRIVATE clp
		PRIVATE ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
	)

	SET_TARGET_PROPERTIES( unittest 
		PROPERTIES 
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests 
	)

	ADD_TEST( NAME clp-unittest COMMAND unittest )
ENDIF()

#
# Install
#
INSTALL(
	TARGETS clp
	PUBLIC_HEADER DESTINATION include/elrat/clp
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)
