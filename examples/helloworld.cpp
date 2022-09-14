#include <elrat/clp.hpp>
#include <cctype>
#include <iostream>

using namespace elrat::clp;

void sayHello(const CommandLine& cmdline)
{
    static const auto toUpperCase{[](std::string& s){
        static const int offset{32};
        for( auto& it : s )
            if ( it > 'a' && it < 'z' )
                it -= offset;
    }};
    std::string msg("Hello ");
    msg += cmdline.getCommandParameter(0);
    if ( cmdline.optionExists("shout") )
        toUpperCase(msg);
    std::cout << msg << '\n';
}

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

const std::vector<std::string> input_sequences {
     "sayhello World"   
    ,"sayhello World --shout"
    ,"sayhello --shout You" 
    ,"sayhello not@name"        // @ is an invalid character in 'ParameterType::Name'
    ,"sayhello"                 // Missing parameter which is declared 'Mandatory'
    ,"sayhello world --whisper" // Undefined option
    ,"sayhello me"              // Disallowed parameter value
    ,"say_hello world"          // Unrecognized command
};

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
   
    processor.attach(
        createSayHelloDescriptor(), 
        sayHello);

    for(auto& line : input_sequences ) {
        std::cout << "> " << line << "\n";
        try {
            processor.process(line);
        }
        catch( InputException& exception ) {
            std::cout << exception.what() << '\n';
        }
    }
    return 0;
}
