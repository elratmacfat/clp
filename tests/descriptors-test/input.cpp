#include "descriptors-test/input.hpp"
#include "descriptors-test/util.hpp"

using namespace elrat::clp;

namespace ParameterValidation
{
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
	     "a:"
	    ,"A:\\"  
	    ,"Z:"
	    ,"relative\\path\\"
	    ,"relative\\path-to\\executable.exe"
	    ,"path\\containing spaces\\"
	    ,"C:\\Program Files\\program_v1.exe" 
	};
	const std::vector<std::string> nix_paths {
	     "/home/elrat/../elrat/.vim/vimrc" 
	    ,"elrat/Desktop"
	    ,".hidden/directory/"
	    ,".abc-def_x"
	    ,"relative path/containing spaces/"
	};
	
    const std::vector<std::string> email_addresses {
         "hello.world@server.com"
        ,"hello-world@server.com"
        ,"hello_world@server.com"
        ,"a@a.a"
        ,"a@a-a.a"
        ,"a@a_a.a"
    };

    const std::vector<std::string> invalid_email_addresses {
         "helloworld@@server.com"
        ,"hello/world@server.com"
        ,"abc?def@server.com"
        ,"-abc@xxx.com"
        ,"abc-@xxx.com"
        ,"abc.-abc@abc.com"
        ,"abc-def@abc-.com"
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
} // namespace ParameterValidation

namespace OptionValidation
{
	OptionDescriptorPtr createOptionDescriptor() {
	    return OptionDescriptor::Create( "option", "description", {
            ParameterDescriptor::Create(
                "p0", "description",
                Mandatory,
                ParameterType::NaturalNumber, {
                    AtLeast(10),
                    Not(20)
                }
            ),
            ParameterDescriptor::Create(
                "p1", "p1description",
                Mandatory,
                ParameterType::WholeNumber, {
                    AtLeast(-100),
                    Not(0)
                }
            ),
            ParameterDescriptor::Create(
                "p2", "p2description",
                Optional,
                ParameterType::Identifier, {
                    Not<std::string>("help")
                }
            )
	    });
	}
	
	const std::vector<std::vector<std::string>> valid_parameters {
	     {"10","-100","some_identifier"}
	    ,{"99","-10", "__x"}
	    ,{"99","-10"}
	};

    const std::vector<std::vector<std::string>> missing_parameters {
         {}
        ,{"10"}
    };

    const std::vector<std::vector<std::string>> too_many_parameters {
         {"10","-100","some_identifier","and_one_too_many"}
        ,{"10","-100","some_identifier","1","2","3 too many"}
    };
    
    const std::vector<std::vector<std::string>> invalid_parameter_types {
         {"-10","-100"}
	    ,{"invalidtext","-100","identifier"}
	    ,{"10","invalidtext","name"}
	    ,{"10","-100","invalud-identifier"}
	};

    const std::vector<std::vector<std::string>> invalid_parameter_values {
         {"5","-100"}
        ,{"20","-100"}
        ,{"10","-101"}
        ,{"10","0"}
        ,{"10","-100","help"}
    };

} // namespace OptionValidation

namespace CommandValidation
{
	CommandDescriptorPtr createCommandDescriptor() 
    {
	    return CommandDescriptor::Create(
	        "cube",
	        "create a customized cube.", 
            {
	            ParameterDescriptor::Create(
	                "size", 
	                "Edge length",
	                Mandatory,
	                ParameterType::RealNumber
                )
            },
            {
	            OptionDescriptor::Create(
	                "color",
	                "Defines the surface color of the cube.", 
                    {
	                    ParameterDescriptor::Create(
	                        "clr",
	                        "color",
	                        Mandatory,
	                        ParameterType::Any, 
                            {
	                            In<std::string>("red","blue","green")
                            }
                        )
                    }
                ),
                OptionDescriptor::Create(
                    "ice",
                    "Create an IceCube instead."
                )
            }
	    );
	}
	
    std::vector<CommandLine> createValid()
	{
	    std::vector<CommandLine> cls(3);
        for( auto& c : cls ) 
        {
            c.setCommand("cube");
            c.addCommandParameter("12.1");
        }
        
        cls[1].addOption("color");
        cls[1].addOptionParameter("red");

        cls[2].addOption("ice");

        return std::move(cls);
	}
	
    std::vector<CommandLine> createUnrecognized()
    {
        std::vector<CommandLine> cls(3);
        cls[0].setCommand("hello");
        cls[1].setCommand("world");
        cls[1].addCommandParameter("1234");
        cls[2].setCommand("__test");
        cls[2].addOption("option");
        return std::move(cls);
    }

    std::vector<CommandLine> createMissingParameters()
    {
        std::vector<CommandLine> cls(1);
        cls[0].setCommand("cube");
        return cls;
    }
    
    std::vector<CommandLine> createTooManyParameters()
    {
        std::vector<CommandLine> cls(2);
        for(auto& c : cls)
        {
            c.setCommand("cube");
            c.addCommandParameter("12");
            c.addCommandParameter("13");
        }
        cls[1].addCommandParameter("14");
        return cls;
    }

} // namespace CommandValidation
