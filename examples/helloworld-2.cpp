#include <elrat/clp/clp.hpp>
#include <cctype>
#include <iostream>

using namespace elrat::clp;

void sayHelloFunction(const CommandLine&);
CommandDescriptorPtr createSayHelloDescriptor();
std::vector<std::string> createInputSequences();

int main()
{
    auto sayHelloDescriptor{ createSayHelloDescriptor() };
    auto inputSequences{ createInputSequences() };
    
    Processor processor;                                
    processor.attach( 
        sayHelloDescriptor, 
        sayHelloFunction );

    for(auto& input: inputSequences ) 
    {
        std::cout << "> " << input << '\n';
        try 
        {
            processor.process( input );
        }
        catch( InputException& exception ) 
        {
            std::cout << exception.what() << '\n';
        }
    }
    return 0;
}

void sayHelloFunction(const CommandLine& cmdline)
{
    static const auto toUpperCase{[](std::string& s){
        static const int offset{32};
        for( auto& it : s )
            if ( it >= 'a' && it <= 'z' )
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
                    Not<std::string>("me")
                })
        }, {
            OptionDescriptor::Create("shout")
        }
    );
}

std::vector<std::string> createInputSequences() 
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
