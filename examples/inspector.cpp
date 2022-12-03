#include "elrat/clp/clp.hpp"

#include <iostream>
#include <cstdio>

int main() {
    using elrat::clp::Parser;
    using elrat::clp::NativeParser;
    using elrat::clp::CommandLine;

    const std::string szQuit("q");
    NativeParser parser;

    std::cout << "Syntax:\n   "
        << parser.getSyntaxDescription()
        << "\nType '"
        << szQuit
        << "' to quit.\n";

    std::string s;
    do {
        std::cout << "> ";
        std::getline( std::cin, s );
        try {    
            CommandLine data{ parser.parse(s) };
            std::cout << data << '\n';
        }
        catch( std::exception& exc ) {
            std::cout << exc.what() << "\n";
        }
    } while( s != szQuit );

    return 0;
}
