
#include <elrat/clp.h>

using namespace elrat::clp;
using namespace std;

// Setting up the command descriptors is busy work, but it's got to be done.
// I'd recommend creating a separate *.cpp file for this purpose, but as for
// this example, the implementation is done right below main().
CommandDescriptorMap initialize_command_descriptors();


// All command function need the same function header. Instead of passing
// several arguments individually, we declare a structure, that contains
// all the information relevant to the application logic.
typedef struct ApplicationData {
	ApplicationData( ostream& osDest, ostream& osErr, istream& isSrc, CmdDescMap& cdm )
	: os_dest{osDest}
	, os_err{osErr}
	, is_src{isSrc}
	, cmd_desc_map{cdm}
	{
		// work done in member initializer list
	}
	
	ostream& os_dest;			// Textual output will be written here, usually std::cout
	ostream& os_err;			// Error message will be redirected here, usually std::cerr
	istream& is_src;			// Read textual input from here, usually std::cin
	CmdDescMap& cmd_desc_map;	// The help-command needs access to the command descriptors.
								// Admitted, this isn't a neat approach, but for the example
								// it has to suffice.
	CommandStrings cmd_str;		// The parsed command line
} AppData;

// All our command function will return a boolean (true for success, otherwise false).
// They expect a reference to the application data, where they find the command line
// input as well as the output stream etc etc...
typedef bool(*Callback)( AppData& );

// These are our own Command.
bool makesum( AppData& app_data );	// Make a sum of two doubles. Descriptor and command implementation below main()
bool help( AppData& app_data );		 // Wraps around the library's pre-implemented printHelpMessage()-function to match our function header.

int main( int argc, char* argv[] )
{
	//
	//	Initialization
	//
	CmdDescMap cmd_desc_map{ initialize_command_descriptors() };

	ApplicationData app_data(cout,cerr,cin,cmd_desc_map);

	CmdMap<Callback> cmd_map;
	cmd_map.add("help",help);
	cmd_map.add("makesum",makesum);

	// At this point the initialization is complete.
	// Assume we get some command lines from stdin, a file, a script, does not matter
	vector<string> vszCmd {
		"help",						// OK
		"help makesum --long",		// OK
		"makesum -5.2 15.9",		// OK
		"makesum -5.2 2.1",			// FAILS, invalid argument (ErrorCode == 6). Second argument must be >= 10.0 (see descriptor below)
		"dummy",					// OK, but does nothing
		"Do something."				// FAILS, invalid Command (ErrorCode == 2)
	};

	// Go over the command lines and try to call the respective functions from the command map.
	for( auto& szCmd : vszCmd ) {
		// Echo the command, as if we typed it ourselves.
		cout << "> " << szCmd << "\n";
		// The CommandStrings class parses the elements following the syntax rules
		// documented in 'elrat/clp.h'
		app_data.cmd_str = CommandStrings( szCmd );

		// Pass the CommandStrings object to our CommandDescriptorMap, and check if
		// the issued command matches one of the descriptors.
		ErrorCode ec = cmd_desc_map.validate( app_data.cmd_str );
		if ( ec == SUCCESS ) {

			// It is up to the application programmer what to do next.
			// He/She can either make use of the callback mechanism provided 
			// by CommandMap, or handle the execution of a response directly.
			// In any case, at this point it is known that the CommandStrings
			// object does yield values meeting the requirements defined in
			// the command descriptor.
			Callback cbFunc = cmd_map.get( app_data.cmd_str.getCommandName() );
			if ( cbFunc )
				cbFunc( app_data );
			else
				// No command attached, but we still know, that
				// the issued command is valid and the
				// CommandStrings object yields validated values
				// for options and parameters.
				cout << "OK\n";
		}
		else
			cout << "Error: " << ec << "\n";
	}
	cout << "End of example program\n";
	return 0;
}

CommandDescriptorMap initialize_command_descriptors()
{
	// The "help" command is added automatically
	return CommandDescriptorMap("Available commands",
	{
		CommandDescriptor(
			"makesum", "Calculates the sum of two numbers.",
			{
				parameter<NumR>("a","First number",Mandatory),
				parameter<NumR>("b","Second number",Mandatory,{constraint<Min,double>(10)})
			}
		),
		// Dummy command, that does nothing, but exist
		CommandDescriptor("dummy")
	});
}

bool makesum( AppData& app_data )
{
	double a = app_data.cmd_str.getParameterAs<double>(0,0);
	double b = app_data.cmd_str.getParameterAs<double>(0,1);
	app_data.os_dest << a << " + " << b << " = " << a+b << "\n";
	return true;
}

bool help( AppData& app_data )
{
	return printHelpMessage(
				app_data.cmd_str,
				app_data.cmd_desc_map,
				app_data.os_dest );
}

