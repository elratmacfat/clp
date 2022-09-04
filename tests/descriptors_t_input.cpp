#include "descriptors_t_input.hpp"
#include "descriptors_t_util.hpp"

using namespace elrat::clp;

const std::vector<std::string> hexadecimal_zero {
    "0x0", "0X0", "0x0000"
};

const std::vector<std::string> hexadecimals {
    "0x0A", "0xF1"
};

const std::vector<std::string> positive_zero {
    "0", "000", "+0", "+000"
};

const std::vector<std::string> negative_zero {
    "-0", "-000"
};

const std::vector<std::string> positive_integers {
    "39", "+39", "1337", "+1337"
};

const std::vector<std::string> negative_integers {
    "-39", "-1337"
};

const std::vector<std::string> floating_point_numbers {
    "0.0", ".0", "+.0", "-.0", "1.337", "-1.337", "+1.337"
};

const std::vector<std::string> identifiers {
    "identifier", "iden_ti_fier", "_identifier_", "a123", "a_123", "_a_123_"
};

const std::vector<std::string> names {
    "names-can-have-dashes--in-between"
};

const std::vector<std::string> win_paths {
    "C:\\Program Files\\program_v1.exe" 
};
const std::vector<std::string> nix_paths {
    "/home/elrat/../elrat/.vim/vimrc" 
};

const std::vector<std::string> range_0_to_9{ 
    convertRangeToStrings(0,9,1) 
};

const std::vector<std::string> range_10_to_19 { 
    convertRangeToStrings(10,19,1) 
};

const std::vector<std::string> range_20_to_29 { 
    convertRangeToStrings(20,29,1) 
};

CommandDescriptorPtr createCommandAddPlatform()
{
    return makeCommandDescriptor(
        "add_platform",
        "Add a new diving platform to your swimming pool",
        {
            makeParameterDescriptor(
                "height", 
                "Diving platform's height (meter) above the water surface.",
                Mandatory,
                ParameterType::NaturalNumber, {
                    In(1,3,5,10)
                }
            )
        },
        {
            makeOptionDescriptor(
                "color",
                "Use this option to specify a non-standard color for the platform.", 
                {
                    makeParameterDescriptor(
                        "color",
                        "The platform's color.",
                        Mandatory,
                        ParameterType::Any, {
                            In<std::string>("red","blue","green")
                        }
                    )
                }
            )
        }
    );
}

CommandDescriptorPtr createCommandSetTemperature()
{
    return makeCommandDescriptor(
        "set_temperature",
        "Configure the swimming pool's water temperature.", 
        {
            makeParameterDescriptor(
                "temperature",
                "Water temperature in °C.",
                Mandatory,
                ParameterType::RealNumber, {
                    AtLeast(0.0f)
                }
            )
        },
        {
            makeOptionDescriptor(
                "turn-off",
                "Pass this option to turn off the heater at night." 
            )
        }
    );
}

CommandDescriptorsPtr createCommandDescriptors()
{
    auto cmddescs{ CommandDescriptors::Create() };
    cmddescs->attach( createCommandAddPlatform() );
    cmddescs->attach( createCommandSetTemperature() );
    return cmddescs;
}

std::vector<CommandLine> createValidCommandLines()
{
    std::vector<CommandLine> result{};
    
    CommandLine c0;
    c0.setCommand("add_platform");
    c0.addCommandParameter("5");
    c0.addOption("color");
    c0.addOptionParameter("red");

    CommandLine c1;
    c1.setCommand("add_platform");
    c1.addCommandParameter("3");

    CommandLine c2;
    c2.setCommand("set_temperature");
    c2.addCommandParameter("21.5");
    c2.addOption("turn-off");

    return std::vector<CommandLine>{c0,c1,c2};
}

std::vector<CommandLine> createInvalidCommandLines()
{
    std::vector<CommandLine> result{};
    
    CommandLine c0;
    c0.setCommand("add_platform");
    c0.addCommandParameter("5");
    c0.addOption("color");
    c0.addOptionParameter("yellow"); // invalid

    CommandLine c1;
    c1.setCommand("add_platform");
    c1.addCommandParameter("4");    // <--

    CommandLine c2;
    c2.setCommand("set_temperature");
    c2.addCommandParameter("21.5");
    c2.addOption("turn-off");
    c2.addOption("blah");   // <--

    CommandLine c3;
    c3.setCommand("blah"); // <--

    return std::vector<CommandLine>{c0,c1,c2,c3};
}


