// Project......: Command Line Processor (clp)
// File.........: src/tests/descriptors_t.cpp
// Author.......: elratmacfat
// Description..: 
//
#define BOOST_TEST_MODULE "CLP - Descriptors"
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <elrat/clp.hpp>

#include "descriptors_t_util.hpp"
#include "descriptors_t_input.hpp"

// 
//
//
BOOST_AUTO_TEST_SUITE( UTIL_SELFTEST )
    BOOST_AUTO_TEST_CASE( CONVERT_INTEGER_RANGE_TO_STRINGS )
    {
        std::vector<std::string> integer_strings = convertRangeToStrings(1,6,2);
        BOOST_REQUIRE_EQUAL( integer_strings.size(), 3 );
        BOOST_CHECK_EQUAL( integer_strings[0], "1" );
        BOOST_CHECK_EQUAL( integer_strings[1], "3" );
        BOOST_CHECK_EQUAL( integer_strings[2], "5" );
    }
BOOST_AUTO_TEST_SUITE_END(); // UTIL_SELFTEST

//
//
//
BOOST_AUTO_TEST_SUITE( PARAMETER_VALIDATION )
   
    using namespace elrat::clp;

    //
    //
    //
    BOOST_AUTO_TEST_SUITE( TYPE )

        BOOST_AUTO_TEST_CASE( NATURAL_NUMBER )
        {
            Check( ParameterType::NaturalNumber, positive_zero );
            Check( ParameterType::NaturalNumber, positive_integers );
            Check( ParameterType::NaturalNumber, hexadecimal_zero );
            Check( ParameterType::NaturalNumber, hexadecimals );
            FailCheck( ParameterType::NaturalNumber, negative_integers );
            FailCheck( ParameterType::NaturalNumber, floating_point_numbers );
        }
    
        BOOST_AUTO_TEST_CASE( WHOLE_NUMBER )
        {
            Check( ParameterType::WholeNumber, positive_zero );
            Check( ParameterType::WholeNumber, negative_zero );
            Check( ParameterType::WholeNumber, positive_integers );
            Check( ParameterType::WholeNumber, negative_integers );
            Check( ParameterType::WholeNumber, hexadecimal_zero );
            Check( ParameterType::WholeNumber, hexadecimals );
            FailCheck( ParameterType::WholeNumber, floating_point_numbers );
        }
        
        BOOST_AUTO_TEST_CASE( REAL_NUMBER )
        {
            Check( ParameterType::RealNumber, positive_zero );
            Check( ParameterType::RealNumber, negative_zero );
            Check( ParameterType::RealNumber, positive_integers );
            Check( ParameterType::RealNumber, negative_integers );
            Check( ParameterType::RealNumber, floating_point_numbers );
        }
    
        BOOST_AUTO_TEST_CASE( IDENTIFIER )
        {
            Check( ParameterType::Identifier, identifiers );
            FailCheck( ParameterType::Identifier, names );
        }
    
        BOOST_AUTO_TEST_CASE( NAME )
        {
            Check( ParameterType::Name, identifiers );
            Check( ParameterType::Name, names );
        }
    
        BOOST_AUTO_TEST_CASE( PATH )
        {
            Check( ParameterType::Path, win_paths );
            Check( ParameterType::Path, nix_paths );
        }
    
    BOOST_AUTO_TEST_SUITE_END(); // TYPE 
        
    //
    //
    //
    BOOST_AUTO_TEST_SUITE( CONSTRAINTS_INT )
        
    
        BOOST_AUTO_TEST_CASE( AT_LEAST )
        {
            auto constraint{ AtLeast(10) };
            FailCheck( constraint, range_0_to_9);
            Check( constraint, range_10_to_19 );
        }
    
        BOOST_AUTO_TEST_CASE( AT_MOST )
        {
            auto constraint{ AtMost(9) };
            Check( constraint, range_0_to_9 );
            FailCheck( constraint, range_10_to_19 );
        }
    
        BOOST_AUTO_TEST_CASE( IN_RANGE )
        {
            auto constraint{ InRange(10,19) };
            Check( constraint, range_10_to_19 );
            FailCheck( constraint, range_0_to_9 );
            FailCheck( constraint, range_20_to_29 );
        }
    
        BOOST_AUTO_TEST_CASE( NOT )
        {
            auto constraint{ Not(10) };
            Check( constraint, range_0_to_9 );
            BOOST_CHECK( !constraint->validate("10") );
        }
    
        BOOST_AUTO_TEST_CASE( IN )
        {
            auto constraint{ In(10,12,14) };
            BOOST_CHECK( constraint->validate("10") );
            BOOST_CHECK( constraint->validate("12") );
            BOOST_CHECK( constraint->validate("14") );
            BOOST_CHECK( !constraint->validate("11") );
            BOOST_CHECK( !constraint->validate("13") );
            BOOST_CHECK( !constraint->validate("15") );
        }
    
    BOOST_AUTO_TEST_SUITE_END(); // CONSTRAINTS_INT

    //
    //
    //
    BOOST_AUTO_TEST_SUITE( CONSTRAINTS_FLOAT )
        
        using namespace elrat::clp;

        const std::vector<std::string> fm10 { "-10.000", "-10.0", "-10" };
        const std::vector<std::string> f0 { "0.00", ".0000", "0" };
        const std::vector<std::string> f10 { "+10.00", "10.000", "10" };
        
        BOOST_AUTO_TEST_CASE( AT_LEAST )
        {
            auto constraint{ AtLeast(0.0f) };
            FailCheck( constraint, fm10 );
            Check( constraint, f0 );
            Check( constraint, f10 );
        }

        BOOST_AUTO_TEST_CASE( AT_MOST )
        {
            auto constraint{ AtMost(0.0f) };
            Check( constraint, fm10 );
            Check( constraint, f0 );
            FailCheck( constraint, f10 );
        }

        BOOST_AUTO_TEST_CASE( IN_RANGE )
        {
            auto constraint{ InRange( 0.0f, 10.0f ) };
            FailCheck( constraint, fm10 );
            Check( constraint, f0 );
            Check( constraint, f10 );
        }

        BOOST_AUTO_TEST_CASE( NOT )
        {
            auto constraint{ Not( 0.0f ) };
            Check( constraint, fm10 );
            FailCheck( constraint, f0 );
            Check( constraint, f10 );
        }

        BOOST_AUTO_TEST_CASE( IN )
        {
            auto constraint{ In( -10.0f, 0.0f, 10.f ) };
            Check( constraint, fm10 );
            Check( constraint, f0 );
            Check( constraint, f10 );
            constraint = In( -10.1f, 0.1f, 10.1f );
            FailCheck( constraint, fm10 );
            FailCheck( constraint, f0 );
            FailCheck( constraint, f10 );

        }

    BOOST_AUTO_TEST_SUITE_END() // CONSTRAINTS_FLOAT 
    
    //
    //
    //
    BOOST_AUTO_TEST_SUITE( CONSTRAINTS_STRING )
        
        const std::vector<std::string> valid_strings{
            "abc","def","ghi"
        };

        const std::vector<std::string> invalid_strings{
            "cba","fed","ihg"
        };

        BOOST_AUTO_TEST_CASE( IN )
        {
            auto constraint{ In<std::string>("abc","def","ghi") };
            Check( constraint, valid_strings );
            FailCheck( constraint, invalid_strings );
        }

    BOOST_AUTO_TEST_SUITE_END() // CONSTRAINTS_STRING

BOOST_AUTO_TEST_SUITE_END(); // PARAMETER_VALIDATION

//
//
//
BOOST_AUTO_TEST_SUITE( OPTION_VALIDATION )
    using namespace elrat;
    using namespace elrat::clp;
    using Parameters = std::vector<std::string>;
    using Candidates = std::vector<Parameters>;

   
    auto opt_desc = makeOptionDescriptor(
        "option",
        "description", {
            makeParameterDescriptor(
                "p0", 
                "p0description",
                Mandatory,
                ParameterType::NaturalNumber, {
                    AtLeast(10),
                    Not(20)
                }
            ),
            makeParameterDescriptor(
                "p1", "p1description",
                Mandatory,
                ParameterType::WholeNumber, {
                    AtLeast(-100),
                    Not(0)
                }
            ),
            makeParameterDescriptor(
                "p2", "p2description",
                Optional,
                ParameterType::Identifier, {
                    Not<std::string>("help")
                }
            )
        });

    const Candidates valid_candidates {
         Parameters{"10","-100","some_identifier"}
        ,Parameters{"99","-10", "__x"}
        ,Parameters{"99","-10"}
    };

    const Candidates invalid_candidates {
         Parameters{"10"}
        ,Parameters{"10","-101","__x"}
        ,Parameters{"20","-100"}
        ,Parameters{"10","-100","invalid-identifier"}
        ,Parameters{"99","-10","help"}
    };

    BOOST_AUTO_TEST_CASE( PREREQUISITES )
    {
        BOOST_CHECK_EQUAL( opt_desc->getRequiredParameterCount(), 2 );
    }

    BOOST_AUTO_TEST_CASE( VALID_CANDIDATES )
    {  
        for( auto& candidate : valid_candidates )
            Check( opt_desc, candidate );    
    }

    BOOST_AUTO_TEST_CASE( INVALID_CANDIDATES )
    {   
        for( auto& candidate : invalid_candidates )
            FailCheck( opt_desc, candidate );    
    }

BOOST_AUTO_TEST_SUITE_END(); // OPTION_VALIDATION

//
//
//
BOOST_AUTO_TEST_SUITE( COMMAND_VALIDATION )
    using namespace elrat;
    using namespace elrat::clp;
    
    auto cmddescs{ createCommandDescriptors() };

    std::vector<CommandLine> valid_cmdlines{ createValidCommandLines() };
    std::vector<CommandLine> invalid_cmdlines{ createInvalidCommandLines() };

    BOOST_AUTO_TEST_CASE( VALID )
    {
        for( int i{0}; i<valid_cmdlines.size(); i++ )
        {
            ValidationResult result{ cmddescs->validate(valid_cmdlines[i]) };
            BOOST_CHECK_MESSAGE( clp::isMatch(result),
                "valid_cmdlines[" << i << "]: " << toString(result)  );
        }
    }

    BOOST_AUTO_TEST_CASE( INVALID )
    {
        for( int i{0}; i<invalid_cmdlines.size(); i++ )
        {
            ValidationResult result{ cmddescs->validate(valid_cmdlines[i]) };
            BOOST_CHECK_MESSAGE( clp::isNoMatch(result) || isInvalid(result),
                "invalid_cmdlines[" << i << "]: " << toString(result) );
        }
    }

BOOST_AUTO_TEST_SUITE_END(); // COMMAND_VALIDATION 


