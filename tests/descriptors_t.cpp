// Project......: Command Line Processor (clp)
// File.........: src/tests/descriptors_t.cpp
// Author.......: elratmacfat
// Description..: 
//
#define BOOST_TEST_MODULE "CLP - Descriptors"
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <elrat/clp.hpp>

using namespace elrat;
using namespace elrat::clp;

//
//
//
BOOST_AUTO_TEST_SUITE( Parameter_Type_Test_Suite )

    using Candidates = std::vector<std::string>;

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

    BOOST_AUTO_TEST_CASE( Natural_Number_Test_Case )
    {
        Check( ParameterType::NaturalNumber, positive_zero );
        Check( ParameterType::NaturalNumber, positive_integers );
        CheckFail( ParameterType::NaturalNumber, negative_integers );
        CheckFail( ParameterType::NaturalNumber, floating_point_numbers );
    }

    BOOST_AUTO_TEST_CASE( Whole_Number_Test_Case )
    {
        Check( ParameterType::WholeNumber, positive_zero );
        Check( ParameterType::WholeNumber, negative_zero );
        Check( ParameterType::WholeNumber, positive_integers );
        Check( ParameterType::WholeNumber, negative_integers );
        CheckFail( ParameterType::WholeNumber, floating_point_numbers );
    }
    
    BOOST_AUTO_TEST_CASE( Real_Number_Test_Case )
    {
        Check( ParameterType::RealNumber, positive_zero );
        Check( ParameterType::RealNumber, negative_zero );
        Check( ParameterType::RealNumber, positive_integers );
        Check( ParameterType::RealNumber, negative_integers );
        Check( ParameterType::RealNumber, floating_point_numbers );
    }

    BOOST_AUTO_TEST_CASE( Identifier_Test_Case )
    {
        Check( ParameterType::Identifier, identifiers );
        CheckFail( ParameterType::Identifier, names );
    }

    BOOST_AUTO_TEST_CASE( Name_Test_case )
    {
        Check( ParameterType::Name, identifiers );
        Check( ParameterType::Name, names );
    }

    BOOST_AUTO_TEST_CASE( Path )
    {
        Check( ParameterType::Path, win_paths );
        Check( ParameterType::Path, nix_paths );
    }

BOOST_AUTO_TEST_SUITE_END(); // Parameter_Type_Test_Suite
    
//
//
//
BOOST_AUTO_TEST_SUITE( Constraints_Test_Suite )

    using Candidates = std::vector<std::string>; 
    
    template <class T>
    Candidates createCandidates(T from, T to, T increment ) {
        Candidates result{};
        T current{from};
        while ( current <= to )
        {
            result.push_back(std::to_string(current));
            current+=increment;
        }
        return result;
    }

    void Check( ConstraintPtr constraint, const Candidates& candidates )
    {
        for( auto& candidate : candidates ) 
            BOOST_CHECK_MESSAGE( constraint->validate(candidate), candidate );
    }

    void CheckFail( ConstraintPtr constraint, const Candidates& candidates )
    {
        for( auto& candidate : candidates ) 
            BOOST_CHECK_MESSAGE( !(constraint->validate(candidate)), candidate );
    }

    const Candidates zero_to_nine{ createCandidates(0,9,1) };
    const Candidates ten_to_nineteen{ createCandidates(10,19,1) };

    BOOST_AUTO_TEST_CASE( At_Least )
    {
        auto constraint{ AtLeast(10) };
        CheckFail( constraint, zero_to_nine );
        Check( constraint, ten_to_nineteen );
    }

    BOOST_AUTO_TEST_CASE( At_Most )
    {
        BOOST_CHECK(false);
    }

    BOOST_AUTO_TEST_CASE( In_Range )
    {
        BOOST_CHECK(false);
    }

    BOOST_AUTO_TEST_CASE( Is_Not )
    {
        BOOST_CHECK(false);
    }

    BOOST_AUTO_TEST_CASE( In )
    {
        BOOST_CHECK(false);
    }


BOOST_AUTO_TEST_SUITE_END(); // Constraints_Test_Suite
