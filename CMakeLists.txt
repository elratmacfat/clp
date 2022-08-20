CMAKE_MINIMUM_REQUIRED( VERSION 3.10 )

PROJECT( clp CXX )

SET( CMAKE_CXX_STANDARD 17 )

#
# Library
#

SET( HeaderFiles
    inc/elrat/clp/parser.hpp
    inc/elrat/clp/text.hpp
    inc/elrat/clp/desc.hpp
    inc/elrat/clp/util.hpp
    inc/elrat/clp/regex.hpp
)

SET( SourceFiles 
    src/parser.cpp
    src/text.cpp
    src/desc.cpp
    src/regex.cpp
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
        src/tests/class-level-test/main-clt.cpp
        src/tests/class-level-test/desc-clt.cpp
        src/tests/class-level-test/util-clt.cpp
        src/tests/class-level-test/parser-clt.cpp )
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

