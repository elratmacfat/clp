#include "descriptors_t_util.hpp"
#include <boost/test/unit_test.hpp>

using namespace elrat::clp;

void Check( 
    TypeChecker typechecker, 
    const std::vector<std::string>& candidates )
{
    for( auto& candidate : candidates ) 
        BOOST_CHECK_MESSAGE( typechecker(candidate), candidate );
}

void Check( 
    ConstraintPtr constraint, 
    const std::vector<std::string>& candidates )
{
    for( auto& candidate : candidates ) 
        BOOST_CHECK_MESSAGE( constraint->validate(candidate), candidate );
}

void Check( 
    OptionDescriptorPtr option, 
    const std::vector<std::string>& parameters )
{
    auto result{option->validate(option->getName(), parameters)};
    bool passed{result == ValidationResult::Valid};
    if (!passed) 
        BOOST_CHECK_MESSAGE( passed, toString(parameters) );
    else 
        BOOST_CHECK( passed );
}

void Check( 
    CommandDescriptorPtr cmd_desc, 
    const CommandLine& input,
    ValidationResult expected )
{
    ValidationResult result{ cmd_desc->validate(input) };
    BOOST_CHECK_MESSAGE( result == expected,
        "[" << input << "]: "
        "Expected '" << toString(expected) << "', "
        "but found '" << toString(result) << "'" );
}

void FailCheck( 
    TypeChecker typechecker, 
    const std::vector<std::string>& candidates )
{
    for ( auto& candidate : candidates )
        BOOST_CHECK_MESSAGE( !(typechecker(candidate)), candidate );
}

void FailCheck( 
    ConstraintPtr constraint, 
    const std::vector<std::string>& candidates )
{
    for( auto& candidate : candidates ) 
        BOOST_CHECK_MESSAGE( !(constraint->validate(candidate)), candidate );
}

void FailCheck(
    OptionDescriptorPtr option, 
    const std::vector<std::string>& parameters )
{
    auto result{option->validate(option->getName(), parameters)};
    bool passed{result != ValidationResult::Valid};
    if (!passed) 
        BOOST_CHECK_MESSAGE( passed, toString(parameters) );
    else 
        BOOST_CHECK( passed );
}

std::string toString(ValidationResult validation_result) 
{
    std::string result;
    std::stringstream ss;
    ss << validation_result;
    std::getline(ss,result);
    return result;
}

std::string toString(const std::vector<std::string>& strings) 
{
    std::string result;
    std::stringstream ss;
    for( auto& p : strings )
        ss << p << " ";
    std::getline( ss, result );
    return result;
}

CommandDescriptorsPtr createCommandDescriptors()
{
    CommandDescriptorsPtr cds{ CommandDescriptors::Create() };
    return cds;
}
