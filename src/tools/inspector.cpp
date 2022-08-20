#include <elrat/clp/parser.hpp>
#include <elrat/clp/text.hpp>

#include <iostream>
#include <cstdio>

int main() {
	using namespace elrat;
	const std::string szQuit("q");
    auto p = clp::parser::make<clp::native_parser>();
    std::cout << "Syntax:\n   "
        << p->syntax()
        << "\nType '"
        << szQuit
        << "' to quit.\n";
	std::string s;
	do {
        clp::parser::error err;
		std::cout << "> ";
		std::getline( std::cin, s );
        auto data{ p->parse(s,err) };
        std::cout 
            << err // will do nothing if no error was reported.
            << data
            << '\n';
	} while( s != szQuit );

	return 0;
}
