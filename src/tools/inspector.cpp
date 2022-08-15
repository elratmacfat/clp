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
        bool success = p.parse( s );
        if (!success)
            std::cout << p.getLastError() << '\n';

        std::cout << p << '\n';

	} while( s != szQuit );

	return 0;
}
