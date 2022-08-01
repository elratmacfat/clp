//
// project.......: command line processor (clp)
// file..........: src/example.cpp
// author........: elratmacfat
// description...: demonstration of possible library usage 
//                 
//                 global variables are used to keep the example simple.
//                 
//                 in a more complex context, the commands should proberbly
//                 be realized in classes implementing a command interface.
//                 All required data (that is global in this example), could
//                 be passed to the constructor of the concrete command.
//                
#include <elrat/clp.h>

using namespace elrat::clp;
using namespace std;



// --- 1. Setting up command descriptions -------------------------------------
//
// - "dummy" does nothing. There won't even be an implemented function to it.
// - "sum" will add two numbers.
// - Note that a 'help' command description doesn't need to defined. 
//   It will be added automatically.
//
CommandDescriptorMap cmdDescMap("Available commands", {
	command("dummy"),
	command("sum", 
		"Calculates the sum of two numbers.", {
		parameter<NumR>("a",
			"First number",
			Mandatory),
		parameter<NumR>("b",
			"Second number",
			Mandatory, {
				constraint<Min,double>(10)
			})
		}
	)
});

// --- 2. Defining function, that correspond to the commands ------------------
//
// Implementation of commands. A uniform interface is required. In this
// example we define declare a function signature to be the interface.
// The parsed command line object has to be passed to the function.
using Callback = void(*)( const CommandStrings& );

// The implementation is located below main().
void sum( const CommandStrings& );
void help( const CommandStrings& ); // wraps around the built-in help generator

// 
CommandMap<Callback> cmdMap;


int main( int argc, char* argv[] )
{
	cmdMap.add("help",help);
	cmdMap.add("sum",sum);

//
	// Assume we get some command lines from stdin, a file, a script, ...
	vector<string> vszCmd {
		"help",			// OK
		"help makesum --long",	// OK
		"Do something",		// FAILS, invalid Command
		"sum -5.2 15.9",	// OK
		"sum -5.2 2.1",		// FAILS, invalid argument. 
					// 2nd argument must be >= 10.0 
					// see descriptor below
		"dummy",		// OK, but does nothing
	};

	for( auto& szCmd : vszCmd ) {
		
		// Echo the command, as if we typed it ourselves.
		cout << "\n> " << szCmd << "\n";
		
// --- 3. Parse input ---------------------------------------------------------
//
		// The CommandStrings class parses the elements, separating
		// the command from its parameters and options.
		CommandStrings cmdStr( szCmd );

// --- 4. Validating the input ------------------------------------------------ 
//
		ErrorCode ec = cmdDescMap.validate( cmdStr );
		if ( ec == SUCCESS ) {

// --- 5.+6. Retrieve corresponding function and invoke it --------------------
//
			Callback cb = cmdMap.get(cmdStr.getCommandName());
			if ( cb ) {
				cb( cmdStr );
			}
			else {
				// The 'dummy' command would end up here,
				// because it is a valid command, but there's
				// no function mapped to it. 
				cout << "Command is valid, but no function "
					"is attached.\n";
			}
		}
		else {
			// the input does not satisfy any of the command
			// descriptors.
			cout << "Error: " << ec << "\n";
		}
	}
	return 0;
}

void sum( const CommandStrings& cmdStr )
{
	// If the steps 1. - 6. have been followed properly,
	// it is safe to assume, that the accessed elements
	// exist.
	//
	double a = cmdStr.getParameterAs<double>(0,0);
	double b = cmdStr.getParameterAs<double>(0,1);
	std::cout << a << " + " << b << " = " << a+b << "\n";
}

void help( const CommandStrings& cmdStr )
{
	printHelpMessage( 
		cmdStr, 
		cmdDescMap,
		std::cout 
	);
}

