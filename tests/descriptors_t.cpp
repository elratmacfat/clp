// Project......: Command Line Processor (clp)
// File.........: src/tests/descriptors_t.cpp
// Author.......: elratmacfat
// Description..: 
//
#define BOOST_TEST_MODULE "CLP - Descriptors"
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <elrat/clp.hpp>

BOOST_AUTO_TEST_SUITE( PARAMETER_VALIDATION )
   
    using namespace elrat;

    using Candidates = std::vector<std::string>;
    
    template <class T>
    Candidates createCandidates(T from, T to, T increment ) 
    {
        Candidates result{};
        while ( from <= to )
        {
            result.push_back(std::to_string(from));
            from+=increment;
        }
        return result;
    }

    void Check( clp::TypeChecker typechecker, const Candidates& candidates )
    {
        for( auto& candidate : candidates ) 
            BOOST_CHECK_MESSAGE( typechecker(candidate), candidate );
    }

    void CheckFail( clp::TypeChecker typechecker, const Candidates& candidates )
    {
        for ( auto& candidate : candidates )
            BOOST_CHECK_MESSAGE( !(typechecker(candidate)), candidate );
    }
    
    void Check( clp::ConstraintPtr constraint, const Candidates& candidates )
    {
        for( auto& candidate : candidates ) 
            BOOST_CHECK_MESSAGE( constraint->validate(candidate), candidate );
    }

    void CheckFail( clp::ConstraintPtr constraint, const Candidates& candidates )
    {
        for( auto& candidate : candidates ) 
            BOOST_CHECK_MESSAGE( !(constraint->validate(candidate)), candidate );
    }

    //
    //
    //
    BOOST_AUTO_TEST_SUITE( TYPE )
        
        using namespace elrat::clp;
            
        const Candidates positive_zero {
            "0", "000", "+0", "+000"
        };
        const Candidates negative_zero {
            "-0", "-000"
        };
        const Candidates positive_integers {
            "39", "+39", "1337", "+1337"
        };
        const Candidates negative_integers {
            "-39", "-1337"
        };
        const Candidates floating_point_numbers {
            "0.0", ".0", "+.0", "-.0",
            "1.337", "-1.337", "+1.337"
        };
        const Candidates identifiers {
            "identifier", "iden_ti_fier", "_identifier_",
            "a123", "a_123", "_a_123_"
        };
        const Candidates names {
            "names-can-have-dashes--in-between"
        };
        const Candidates win_paths {
            "C:\\Program Files\\program_v1.exe" };
        const Candidates nix_paths {
            "/home/elrat/../elrat/.vim/vimrc" };
    
        BOOST_AUTO_TEST_CASE( NATURAL_NUMBER )
        {
            Check( ParameterType::NaturalNumber, positive_zero );
            Check( ParameterType::NaturalNumber, positive_integers );
            CheckFail( ParameterType::NaturalNumber, negative_integers );
            CheckFail( ParameterType::NaturalNumber, floating_point_numbers );
        }
    
        BOOST_AUTO_TEST_CASE( WHOLE_NUMBER )
        {
            Check( ParameterType::WholeNumber, positive_zero );
            Check( ParameterType::WholeNumber, negative_zero );
            Check( ParameterType::WholeNumber, positive_integers );
            Check( ParameterType::WholeNumber, negative_integers );
            CheckFail( ParameterType::WholeNumber, floating_point_numbers );
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
            CheckFail( ParameterType::Identifier, names );
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
        
        using namespace elrat::clp;
        
        const Candidates range_0_to_9{ createCandidates(0,9,1) };
        const Candidates range_10_to_19{ createCandidates(10,19,1) };
        const Candidates range_20_to_29{ createCandidates(20,29,1) };
    
        BOOST_AUTO_TEST_CASE( AT_LEAST )
        {
            auto constraint{ AtLeast(10) };
            CheckFail( constraint, range_0_to_9);
            Check( constraint, range_10_to_19 );
        }
    
        BOOST_AUTO_TEST_CASE( AT_MOST )
        {
            auto constraint{ AtMost(9) };
            Check( constraint, range_0_to_9 );
            CheckFail( constraint, range_10_to_19 );
        }
    
        BOOST_AUTO_TEST_CASE( IN_RANGE )
        {
            auto constraint{ InRange(10,19) };
            Check( constraint, range_10_to_19 );
            CheckFail( constraint, range_0_to_9 );
            CheckFail( constraint, range_20_to_29 );
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

        const Candidates fm10 { "-10.000", "-10.0", "-10" };
        const Candidates f0 { "0.00", ".0000", "0" };
        const Candidates f10 { "+10.00", "10.000", "10" };
        
        BOOST_AUTO_TEST_CASE( AT_LEAST )
        {
            auto constraint{ AtLeast(0.0f) };
            CheckFail( constraint, fm10 );
            Check( constraint, f0 );
            Check( constraint, f10 );
        }

        BOOST_AUTO_TEST_CASE( AT_MOST )
        {
            auto constraint{ AtMost(0.0f) };
            Check( constraint, fm10 );
            Check( constraint, f0 );
            CheckFail( constraint, f10 );
        }

        BOOST_AUTO_TEST_CASE( IN_RANGE )
        {
            auto constraint{ InRange( 0.0f, 10.0f ) };
            CheckFail( constraint, fm10 );
            Check( constraint, f0 );
            Check( constraint, f10 );
        }

        BOOST_AUTO_TEST_CASE( NOT )
        {
            auto constraint{ Not( 0.0f ) };
            Check( constraint, fm10 );
            CheckFail( constraint, f0 );
            Check( constraint, f10 );
        }

        BOOST_AUTO_TEST_CASE( IN )
        {
            auto constraint{ In( -10.0f, 0.0f, 10.f ) };
            Check( constraint, fm10 );
            Check( constraint, f0 );
            Check( constraint, f10 );
            constraint = In( -10.1f, 0.1f, 10.1f );
            CheckFail( constraint, fm10 );
            CheckFail( constraint, f0 );
            CheckFail( constraint, f10 );

        }

    BOOST_AUTO_TEST_SUITE_END() // CONSTRAINTS_FLOAT 
    
    //
    //
    //
    BOOST_AUTO_TEST_SUITE( CONSTRAINTS_STRING )
        BOOST_AUTO_TEST_CASE( AT_LEAST )
        {

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

    std::string toString(const Parameters& parameters) 
    {
        std::string result;
        std::stringstream ss;
        for( auto& p : parameters )
            ss << p << " ";
        std::getline( ss, result );
        return result;
    }

    void Check( clp::OptionDescriptorPtr option, const Parameters& parameters )
    {
        bool passed{ option->validate(parameters) == ValidationResult::Valid };
        if (!passed) 
            BOOST_CHECK_MESSAGE( passed, toString(parameters) );
        else 
            BOOST_CHECK( passed );
    }
    
    void CheckFail( clp::OptionDescriptorPtr option, const Parameters& parameters )
    {
        bool passed{ option->validate(parameters) != ValidationResult::Valid };
        if (!passed) 
            BOOST_CHECK_MESSAGE( passed, toString(parameters) );
        else 
            BOOST_CHECK( passed );

    }

    BOOST_AUTO_TEST_CASE( VALID_CANDIDATES )
    {  
        for( auto& candidate : valid_candidates )
            Check( opt_desc, candidate );    
    }

    BOOST_AUTO_TEST_CASE( INVALID_CANDIDATES )
    {   
        for( auto& candidate : invalid_candidates )
            CheckFail( opt_desc, candidate );    
    }

BOOST_AUTO_TEST_SUITE_END(); // OPTION_VALIDATION

