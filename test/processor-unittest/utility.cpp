#include "processor-unittest/utility.hpp"

using namespace elrat::clp;

DescriptorMapPtr initializeDescriptorMapA()
{
    auto map{ DescriptorMap::Create("MapA") };

    map->attach( Add::GetDescriptor() );
    return map;
}

DescriptorMapPtr initializeDescriptorMapB()
{
    auto map{ DescriptorMap::Create("MapB") };
    return map;
}


CommandDescriptorPtr Add::descriptor;

CommandDescriptorPtr Add::GetDescriptor()
{
  if (!descriptor) 
  {
    ParameterDescriptorPtr first_parameter = ParameterDescriptor::Create(
	    "a",
	    "First number",
	    Mandatory,
	    ParameterType::RealNumber
    );
    ParameterDescriptorPtr second_parameter = ParameterDescriptor::Create(
      "b",
  		"Second number",
  		Mandatory,
  		ParameterType::RealNumber
    );
    descriptor = CommandDescriptor::Create("add", "add two numbers", { first_parameter, second_parameter } );
  }
  return descriptor;
}

std::shared_ptr<Add> Add::CreateCommand() 
{
    return std::make_shared<Add>();
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
