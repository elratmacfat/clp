#include <elrat/clp/parser.hpp>

#include <iostream>

#include <cstdio>

int main() {
	using namespace elrat::clp;
	const std::string szQuit("q");
	std::cout << "Type 'q' to quit.\n";	
	std::string s;
	do {
		std::cout << "> ";
		std::getline( std::cin, s );
        Parser p;
        auto err = p.parse( s );
        std::cout 
            << err // will do nothing if no error was reported.
            << p
            << '\n';
	} while( s != szQuit );

	return 0;
}
