#include "processor-test/util.hpp"

using namespace elrat::clp;

DescriptorMapPtr initializeDescriptorMapA()
{
    auto map{ DescriptorMap::Create("MapA") };

    map->attach( CommandDescriptor::Create(
        "add",
        "add two numbers",
        {
            ParameterDescriptor::Create(
                "a",
                "First number",
                Mandatory,
                ParameterType::RealNumber
            ),
            ParameterDescriptor::Create(
                "b",
                "Second number",
                Mandatory,
                ParameterType::RealNumber
            )
        })
    );

    return map;
}

DescriptorMapPtr initializeDescriptorMapB()
{
    auto map{ DescriptorMap::Create("MapB") };
    return map;
}


const std::string Add::name("add");

std::shared_ptr<Add> Add::Create() 
{
    return std::make_shared<Add>();
}

const std::string& Add::GetName()
{
    return name;
}

Add::Add()
: initialized{false}
, result{0.0}
{
}

bool Add::isInitialized() const
{
    return initialized;
}

double Add::getValue() const
{
    return result;
}

void Add::reset()
{
    initialized = false;
    result = 0.0;
}

void Add::execute(const CommandLine& cmdline)
{
    double a = cmdline.getCommandParameterAs<double>(0);
    double b = cmdline.getCommandParameterAs<double>(1);
    result = a + b;
    initialized = true;
}
