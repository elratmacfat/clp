#include <elrat/clp.hpp>
#include <cctype>
#include <iostream>

using namespace elrat::clp;

// Command
// -------
// Prints 'hello' followed by the provided argument. If the option 'shout' is set,
// all letters will be capitalized.
void sayHello(const CommandLine& cmdline)
{
    std::string msg("Hello ");
    msg += cmdline.getCommandParameter(0);
    if ( cmdline.optionExists("shout") )
        for( const char& c : msg )
            std::cout << static_cast<char>(std::toupper(c));
    else
        std::cout << msg;
    std::cout << '\n';
}

// Descriptor
// ----------
// Provides a precise description, which is used to validate issued command lines.
CommandDescriptorPtr createSayHelloDescriptor() 
{
    return CommandDescriptor::Create(
        "sayhello",
        "Greet someone or something", {
            ParameterDescriptor::Create(
                "someone",
                "The one you want to greet.",
                Mandatory,
                ParameterType::Name, {
                    Not<std::string>("me", "you")
                })
        }, {
            OptionDescriptor::Create("shout")
        }
    );
}

// Input 
// -----
// Creates input command lines, some of which are valid. Others are not.
std::vector<std::string> createInput() 
{
    return std::vector<std::string>{
         "sayhello World"   
        ,"sayhello World --shout"
        ,"sayhello --shout You" 
        ,"sayhello not@name"        // @ is an invalid character in 'ParameterType::Name'
        ,"sayhello"                 // Missing parameter which is declared 'Mandatory'
        ,"sayhello world --whisper" // Undefined option
        ,"sayhello me"              // Disallowed parameter value
        ,"say_hello world"          // Unrecognized command
    };
}

// Main
// ----
// Configure the processor (which is default constructed, and thus deploys the built-in
// parser), and let it process the input lines defined above.
// Processing a command line involves the following steps:
// 1. parsing
// 2. validating
// 3. executing
//
int main()
{
    Processor processor;                                
    auto descriptor = createSayHelloDescriptor();      
    processor.attach(descriptor);
    processor.attach(descriptor->getName(), sayHello); 

    auto lines{ createInput() };
    for(auto& line : lines) {
        std::cout << "\n> " << line << "\n";
        try {
            processor.process(line);
        }
        catch( InputException& exception ) {
            std::cout << exception.what() << '\n';
        }
    }
    return 0;
}
