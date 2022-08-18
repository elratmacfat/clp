CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp CXX )

SET( CMAKE_CXX_STANDARD 17 )

#
# Library
#

SET( HeaderFiles
    inc/elrat/clp.hpp
    inc/elrat/clp/parser.hpp
    inc/elrat/clp/data.hpp
    inc/elrat/clp/text.hpp
    inc/elrat/clp/desc.hpp
    inc/elrat/clp/util.hpp
)

SET( SourceFiles 
    src/clp.cpp
    src/parser.cpp
    src/data.cpp
    src/text.cpp
    src/desc.cpp
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

FIND_PACKAGE( Boost COMPONENTS unit_test_framework REQUIRED )

IF ( Boost_FOUND )

	INCLUDE(CTest)
    
    ADD_EXECUTABLE( class-level-test 
        src/tests/class-level-test/clt-main.cpp
        src/tests/class-level-test/clt-desc.cpp
        src/tests/class-level-test/clt-util.cpp
        src/tests/class-level-test/clt-default_parser.cpp )
    TARGET_LINK_LIBRARIES( class-level-test
        PUBLIC clp 
        PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}  )
    ADD_TEST( NAME class-level-test COMMAND class-level-test )

    ADD_EXECUTABLE( component-test
        src/tests/component-test/ct-main.cpp )
    TARGET_LINK_LIBRARIES( component-test
        PUBLIC clp
        PUBLIC ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
    ADD_TEST( NAME component-test COMMAND component-test )

ELSE()
	MESSAGE( WARNING "Could not find 'Boost'. Skip compiling the test executable." )
ENDIF()

