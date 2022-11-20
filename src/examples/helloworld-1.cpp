#include <elrat/clp.hpp>
#include <iostream>

using namespace elrat::clp;

int main()
{
    auto descriptor = CommandDescriptor::Create("sayhello");
    auto function = [](const CommandLine& cmdline) {
        std::cout << "Hello World!\n";
    };
    Processor processor;                                
    processor.attach( descriptor, function );
    processor.process( "sayhello" );
    return 0;
}

