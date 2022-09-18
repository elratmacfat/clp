#include <elrat/clp.hpp>

using namespace elrat::clp;

int main()
{
    const std::vector<std::string> command_lines {
         "help"
        ,"unknown-command"
        ,"exit"
        ,"never-reach-this-command"
    };
    Processor processor;
    for( auto& input : command_lines )
    {
        std::cout << "> " << input << '\n';
        try {
            processor.process( input );
        }
        catch( InputException& exception ) {
            std::cout << exception.what() << '\n';
        }
        std::cout << '\n';
    }
    return 0;
}

