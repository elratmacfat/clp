#include "descriptors_t_input.hpp"
#include "descriptors_t_util.hpp"

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
	OptionDescriptorPtr createOptionDescriptor()
    {
	    return makeOptionDescriptor( "option", "description", {
            makeParameterDescriptor(
                "p0", "description",
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
	    return makeCommandDescriptor(
	        "cube",
	        "create a customized cube.",
	        {
	            makeParameterDescriptor(
	                "size", 
	                "Edge length",
	                Optional,
	                ParameterType::RealNumber)
	        },
	        {
	            makeOptionDescriptor(
	                "color",
	                "Defines the surface color of the cube.", 
	                {
	                    makeParameterDescriptor(
	                        "clr",
	                        "color",
	                        Mandatory,
	                        ParameterType::Any, {
	                            In<std::string>("red","blue","green")
	                        }
	                    )
	                }
	            ),
                makeOptionDescriptor(
                    "ice",
                    "Create an IceCube instead."
                ),

	        }
	    );
	}
	
    std::vector<CommandLine> createValid()
	{
	    std::vector<CommandLine> cls(2);
        for( auto& c : cls ) 
            c.setCommand("cube");
        return std::move(cls);
	}
	
    std::vector<CommandLine> createUnrecognized()
    {
        std::vector<CommandLine> cls(2);
        cls[0].setCommand("hello");
        cls[1].setCommand("world");
        return std::move(cls);
    }
    
} // namespace CommandValidation
