#include <elrat/clp.hpp>
#include <cctype>
#include <iostream>

using namespace elrat;

void sayHello(const clp::CommandLine& cmdline)
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

int main()
{
    clp::CommandDescriptorPtr descriptor = clp::CommandDescriptor::Create (
        "sayhello",
        "Greet someone or something",
        {
            clp::ParameterDescriptor::Create(
                "someone",
                "The one you want to greet.",
                clp::Mandatory,
                clp::ParameterType::Name
            )
        },
        {
            clp::OptionDescriptor::Create("shout")
        }
    );

    clp::Processor processor;

    processor.attach( descriptor );
    processor.attach( descriptor->getName(), sayHello );

    const std::vector<std::string> lines{
         "sayhello World"           // OK
        ,"sayhello World --shout"   // OK
        ,"sayhello --shout World"   // OK
        ,"sayhello not@name"        // @ is an invalid character in 'ParameterType::Name'
        ,"sayhello"                 // Missing parameter which is declared 'Mandatory'
        ,"sayhello world --whisper" // Undefined option
    };

    for(auto& line : lines)
    {
        std::cout << "> " << line << "\n";
        try 
        {
            processor.process(line);
        }
        catch( clp::InputException& exception )
        {
            std::cout << exception.what() << '\n';
        }
        std::cout << '\n';
    }

    return 0;
}
