#include <elrat/clp.h>
#include <cstdio>

int main() {
	using namespace elrat::clp;
	const std::string szQuit("q");
	std::cout << "Type 'q' to quit.\n";	
	std::string szInput;
	do {
		std::cout << "> ";
		std::getline( std::cin, szInput );

		CmdStr c( szInput );
		c.print();

	} while( szInput != szQuit );

	return 0;
}
