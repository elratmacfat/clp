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

    BOOST_AUTO_TEST_CASE( Natural_Number )
    {
        Check( ParameterType::NaturalNumber, positive_zero );
        Check( ParameterType::NaturalNumber, positive_integers );
        CheckFail( ParameterType::NaturalNumber, negative_integers );
        CheckFail( ParameterType::NaturalNumber, floating_point_numbers );
    }

    BOOST_AUTO_TEST_CASE( Whole_Number )
    {
        Check( ParameterType::WholeNumber, positive_zero );
        Check( ParameterType::WholeNumber, negative_zero );
        Check( ParameterType::WholeNumber, positive_integers );
        Check( ParameterType::WholeNumber, negative_integers );
        CheckFail( ParameterType::WholeNumber, floating_point_numbers );
    }
    
    BOOST_AUTO_TEST_CASE( Real_Number )
    {
        Check( ParameterType::RealNumber, positive_zero );
        Check( ParameterType::RealNumber, negative_zero );
        Check( ParameterType::RealNumber, positive_integers );
        Check( ParameterType::RealNumber, negative_integers );
        Check( ParameterType::RealNumber, floating_point_numbers );
    }




BOOST_AUTO_TEST_SUITE_END(); // Parameter_Type_Test_Suite

