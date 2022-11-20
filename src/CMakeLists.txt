
#
# Library
#
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

	ADD_TEST( NAME clp-unittest COMMAND unittest )
	
ENDIF()
