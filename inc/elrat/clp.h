//
// 	clp © elratmacfat, 2021
// 
// 	command line processor
//
// 	file: elrat/clp.h
//
//
// 	Structure of this file:
//
// 		- includes
// 		
// 		- namespace elrat::clp
// 			
// 		- Forward declaration 
// 			- with brief descriptions of what these classes/functions do.
//			
//		- Complete class declaration 
//			- with more detailed descriptions on how to use the class.
//			
//		- Template classes or template members are either implemented within
//		  or right below the declaration.
//
//
#ifndef ELRAT_CLP_H
#define ELRAT_CLP_H

#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>


namespace elrat { 
namespace clp {
	
	//
	//
	//	Types & Constants
	//
	//
	
	// ErrorCode is used to give feedback about issued commands
	//
	typedef enum _ErrorCode {
		SUCCESS = 0,				// Yeah!
		FAILURE,			// 1 	// Something's off, but the author was too lazy to be more communicative about it.
		INVALID_COMMAND,			// The command name is not existent/could not be found in the map.
		INVALID_OPTION, 	// 3	// Option used that is not declared at all (wrong name)
		MISSING_OPTION,     		// Mandatory options are missing.
		TOO_MANY_OPTIONS,	// 5	// More options provided than declared in the descriptor
		INVALID_ARGUMENT,   		// Argument is of wrong type or doesn't match the declared constraints, e.g. a number is too high or low.
		MISSING_ARGUMENT,	// 7	// Too few parameters provided. Some parameters may be mandatory.
		TOO_MANY_ARGUMENTS  	    // Provided arguments may be valid, but there're additional parameters that don't make sense.
	} ErrorCode;

	// When setting up your command descriptors, you have to specify if a parameter is optional or mandatory.
	// Instead of passing a bland 'true' or 'false', you can use these constants, making the call more verbose
	// about what's going on.
	//
	extern const bool Mandatory; // == false
	extern const bool Optional;  // == true

	// 
	//
	// Forward Declaration
	//
	//
	
	// The CmdMap/CommandMap is a std::map with a std::string as key (the command name) and a 'Callback' as value.
	// The 'Callback' can actually be anything. It doesn't even have to be callable. But it should be, if you 
	// follow the intended usage (as shown in src/example.cpp)
	//
	template <class Callback> class CmdMap;
	template <class Callback> using CommandMap = CmdMap<Callback>;

	// CmdStr/CommandStrings is the object that takes a string (e.g. "mycommand 1337 --verbose"), and parses/separates
	// parameters and options. 
	// You can either compile and run src/inspect_cmdstr.cpp to see what the results are, or scroll down to the class
	// declaration.
	//
	class   CmdStr;
	typedef CmdStr      CommandStrings;

	// A command is an aggregate of options and parameters. So are their descriptors.
	//
	class   CmdDesc;
	typedef CmdDesc     CommandDescriptor;

	// The complete description of a command is mapped to its own name, making it easy and fast to find the corresponding
	// description in a typical use case (The user issues a command line, which first 'word' is the command itself).
	//
	class   CmdDescMap;
	typedef CmdDescMap  CommandDescriptorMap;

	// Option Descriptor. Is very similar to a Command Descriptor, but may be declared mandatory or optional (I see the 
	// paradox in the nomenclature, but one could think of 'setting' or 'switch' instead of an option, that might be required
	// in some cases.
	// An option can have several parameters, but does not have to.
	//
	class   OptDesc;
	typedef OptDesc     OptionDescriptor;

	// Each of the following parameter descriptor imply their intended usage and datatype.
	//
	class   ParamDescNumN;          typedef ParamDescNumN           NumN;           // N*   (positive numbers + zero)
	class   ParamDescNumZ;          typedef ParamDescNumZ           NumZ;           // Z    (whole numbers)
	class   ParamDescNumR;          typedef ParamDescNumR           NumR;           // R    (real numbers)
	class   ParamDescIdentifier;    typedef ParamDescIdentifier     Identifier;     //
	class   ParamDescString;        typedef ParamDescString         String;         //

	// Utility free functions
	// Check if the given string represents a number using regular expressions (std::regex)
	//
	bool isHex( const std::string& );				// e.g. 0xAB, 0xf2
	bool isOctal( const std::string& );
	bool isDecimal( const std::string& );			// no leading zeros allowed, because this would indicate an octal representation.
	
	bool isFloatingPoint( const std::string& );		// Any decimal can be interpreted as a floating point.

	// 
	//
	std::string toString( const ErrorCode& );

	//
	//
	std::ostream& operator<<( std::ostream&, const ErrorCode& );

	//
	// Class Declaration And Implementation Of Template Free Functions and Template Members
	// 

	// 
	// 	Constraint
	// 	----------
	//
	//	The Parameter Descriptor (ParamDesc[Num[N|Z|R]|Identifier|String]) itself defines the required data type.
	//	By adding constraints to that descriptor, the range of valid values can be defined. 
	//
	//	Constraint objects are attached to the parameter objects in their constructor call. 
	//
	// 	Example
	//		typedef std::shared_ptr<Constraint> P;
	//
	//  	P p1{ constraint<Min>(5)> };
	//
	//		P p2{ constraint<Max,double>(5.2)> };
	//
	//		P p2{ constraint<Range,double>(5.0,12.4)> };
	
	// This is the interface the specific constraints have to implement.
	//
	class Constraint {
	public:
		virtual ErrorCode check( const std::string& szArg ) const = 0;
		virtual std::string toString() const = 0;
	};

	//  Use this template to create constraints in a readable fashion.
	//  This is shown in detail where the parameter descriptors are 
	//  declared.
	//
	//	Example:
	//		// Create the constraint that the parameter value is expected
	//		// to be ( >= 5 && <= 10 ).
	//		auto constraint_ptr = constraint<Range, int>( 5, 10 );
	//
	//	Template parameters:
	//		_T		is the template parameter of C (this is a syntax related necessatiy).
	//		C		is the specified 'Constraint', namely one of 'Min', 'Max' or 'Range'.
	//		T		This is the data type of the constraint itself and therefor also the  
	//				type of function arguments (constraint values).
	//		Arg		The constraint values
	//				- 'Min' and 'Max' require one argument,
	//				- 'Range' requires two
	//				- Maybe more types will be added, that require any amount of arguments.
	//
	template <template <class _T> class C, class T, class ... Arg>
	std::shared_ptr<Constraint> constraint( Arg ... a ) {
		return std::shared_ptr<Constraint>( new C<T>( a ... ) );
	}

	// Converts the given string into a numerical type.
	// This is an internal helper function.
	// 
	template <class NumType>
	NumType convert( const std::string& szArg ) {
		std::stringstream ss(szArg);
		NumType result{0};
		if ( isDecimal(szArg) )
		ss >> std::dec >> result;
		else if ( isHex(szArg) )
		ss >> std::hex >> result;
		else if ( isOctal(szArg) )
		ss >> std::oct >> result;
		else if ( isFloatingPoint(szArg) )
		ss >> result;
		else
		throw std::runtime_error(
			"template <class NumType> NumType convert(const std::string& szArg)\n"
			"-> Value of 'szArg' could not be converted into a numeric type.\n");
		return result;
	}
	
	// Min-Constraint
	// 
	template <class NumType>
	class Min : public Constraint {
	public:
		Min( NumType arg ) : m_arg{arg} { }

		virtual ErrorCode check( const std::string& szArg ) const {
			NumType t = convert<NumType>(szArg);
			return (t >= m_arg) ? SUCCESS : INVALID_ARGUMENT;
		}

		virtual std::string toString() const {
			return std::string(">=")+std::to_string(m_arg);
		}
	private:
		NumType m_arg;
	};
	
	// Max-Constraint
	//
	template <class NumType>
	class Max : public Constraint {
	public:
		Max( NumType arg ) : m_arg{arg} { }

		virtual ErrorCode check( const std::string& szArg ) const {
			NumType t { convert<NumType>(szArg) };
			return (t <= m_arg) ? SUCCESS : INVALID_ARGUMENT;
		}

		virtual std::string toString() const {
			return std::string("<=")+std::to_string(m_arg);
		}
	private:
		NumType m_arg;
	};
	
	// Range-Constraint
	//
	template <class NumType>
	class Range : public Constraint {
	public:
		Range( NumType arg1, NumType arg2 ) : m_min{arg1}, m_max{arg2} { }

		virtual ErrorCode check( const std::string& szArg ) const {
			NumType t = convert<NumType>(szArg);
			return ( t >= m_min && t <= m_max ) ? SUCCESS : INVALID_ARGUMENT;
		}

		virtual std::string toString() const {
			return std::to_string(m_min) + std::string("...")+ std::to_string(m_max);
		}
	private:
		NumType m_min, m_max;
	};

	//
	// 
	// Descriptor (Desc) 
	// -----------------
	//
	// Base class for command, option and parameter. Provides a 'name' and a descriptive 'what' line.
	class Desc {
	public:
		Desc( const std::string& szName, const std::string& szWhat )
		: m_szName{szName}, m_szWhat{szWhat} { };
		const std::string& getName() const { return m_szName; }
		const std::string& getWhat() const { return m_szWhat; }
	protected:
		const std::string m_szName;
		const std::string m_szWhat;
	};

	//
	//
	// Parameter Descriptor (ParamDesc)
	// --------------------------------
	// 
	// Parameters are attached to Command and Option Descriptors. Each parameters can have
	// a set of constraints.
	//
	// The following is the base class for specified Parameter Descriptors. The individual
	// specifications imply their data type and to some extent their intended usage.
	// E.g. an 'Identifier' is a 'String', but not every 'String' is an 'Identifier'.
	// The specifications contain different regex rules, that determine if a given parameter
	// value meets the requirement.
	//
	class ParamDesc : public Desc {
	public:
		ParamDesc( const std::string& szName,
			const std::string& szWhat = std::string(""),
			bool bOptional = Optional,
			const std::vector<std::shared_ptr<Constraint>>& vpConstraints = {});

		virtual ~ParamDesc();

		bool isOptional() const;

		const std::vector<std::shared_ptr<Constraint>>& getConstraints() const;

		virtual ErrorCode validate( const std::string& szParam ) const = 0;

	protected:
		bool m_bOptional;
		std::vector<std::shared_ptr<Constraint>> m_vpConstraints;
	};

	// Use this template to create parameters for better readability. 
	// As opposed to typing out the std::shared_ptr<> stuff.
	//
	// Example:
	// 		// Assigment omitted, because the return value will probably always be used
	// 		// directly in command/option descriptor's constructor call.
	//  	parameter<NumR>("Radius","The inner circle's radius", MANDATORY, { 
	//  			constraint<Min,double>(0.25) 
	//  	});
	template<class ParameterType>
	std::shared_ptr<ParamDesc> parameter( const std::string& szName,
		const std::string& szWhat = std::string(),
		bool bOptional = Optional,
		const std::vector<std::shared_ptr<Constraint>>& vpConstraints = {})
	{
		return std::shared_ptr<ParamDesc>( new ParameterType(szName,szWhat,bOptional,vpConstraints) );
	}

	// Here are the specified parameter descriptors.
	//
	// Numbers: positive whole numbers + zero (N)
	//
	class ParamDescNumN : public ParamDesc {
	public:
		using ParamDesc::ParamDesc; 	// This means: Use non-default constructor of base class
		virtual ErrorCode validate( const std::string& szParam ) const;
	};


	// Numbers: negative and positive whole numbers. (Z)
	//
	class ParamDescNumZ : public ParamDesc {
	public:
		using ParamDesc::ParamDesc;
		virtual ErrorCode validate( const std::string& szParam ) const;
	};

	// Numbers: Real numbers (R)
	//
	class ParamDescNumR : public ParamDesc {
	public:
		using ParamDesc::ParamDesc;
		virtual ErrorCode validate( const std::string& szParam ) const;
	};

	// String: Identifier (like variable names and such, no special characters allowed,...)
	//
	class ParamDescIdentifier : public ParamDesc {
	public:
		using ParamDesc::ParamDesc;
		virtual ErrorCode validate( const std::string& szParam ) const;
	};

	// String: Path
	// TODO Go to src/clp.cpp and implement the regular expression for the ParamDescPath.
	// Not implemented and not tested. Totally neglected.
	class ParamDescPath : public ParamDesc {
	public:
		using ParamDesc::ParamDesc;
		virtual ErrorCode validate( const std::string& szParam ) const;
	};

	// String: any String
	// Just about any string is accepted as parameters, e.g. a message or something.
	//
	class ParamDescString : public ParamDesc {
	public:
		using ParamDesc::ParamDesc;
		virtual ErrorCode validate( const std::string& szParam ) const;
	};

	//
	//
	// Option Descriptor (OptDesc)
	// ---------------------------
	//
	//
	// Unlike 'Constraint' and 'ParamDesc', this class is not a base class.
	// It contains everything to represent any option.
	//
	class OptDesc : public Desc {
	public:
		OptDesc( const std::string& szName,
			const char& szShortName,
			const std::string& szWhat,
			bool bOptional = Optional,
			const std::vector<std::shared_ptr<ParamDesc>>& vpParameters = {} );

		bool add( const std::shared_ptr<ParamDesc>& pParamDesc );

		unsigned getParamCount() const;
		bool isOptional() const;
		std::string getShortName() const;

		std::shared_ptr<ParamDesc>& operator[]( const std::string& szName );
		std::shared_ptr<ParamDesc>& operator[]( unsigned index );

		const std::shared_ptr<ParamDesc>& getParamDesc( const std::string& szName ) const;
		const std::shared_ptr<ParamDesc>& getParamDesc( unsigned index ) const;

		ErrorCode validate( const std::vector<std::string>& vParameters ) const;

	private:
		const char m_cName;
		const bool m_bOptional;
		std::vector<std::shared_ptr<ParamDesc>> m_vpParamDesc;
	};

	// For better readability of the Command Descriptor's initialization,
	// use this function to construct an Option Descriptor.
	// 
	// Example
	// 		option("message",'m',"Leave a concise, meaningful description of what you've changed.", Mandatory, {
	// 			parameter<String>("msg","commit message")
	// 		});
	//
	OptDesc option(	const std::string& szName,
		const char& szShortName = '\0',
		const std::string& szWhat = std::string(),
		bool bOptional = Optional,
		const std::vector<std::shared_ptr<ParamDesc>>& vpParameters = {} );

	//
	// 
	// Command Descriptor (CmdDesc)
	// ----------------------------
	//
	// This is where it starts coming together.
	//
	// Example:
	//
	// 		command("dump", "Dump the specified state.", {
	// 				parameter<NumN>("state","The state you want to dump. If omitted, the current state is assumed.", Optional )
	// 			},
	// 			{
	// 				option("file",'f',"Instead of printing the state to stdout, write it to a file.", Optional, {
	// 					parameter<String>("filename", "If the file exists, it will be overwritten.", Mandatory )
	// 				})
	// 			}
	// 		);
	//
	class CmdDesc : public Desc {
	public:
		// If you're really lazy, a minimal constructor call looks like CmdDesc("command");
		CmdDesc( const std::string& szName,
			const std::string& szWhat = "",
			const std::vector<std::shared_ptr<ParamDesc>>& vpParameters = {},
			const std::vector<OptDesc>& vOptions = {});

		bool add( const OptDesc& optDesc );

		unsigned getOptCount() const;

		OptDesc& operator[]( const std::string& szName );
		OptDesc& operator[]( unsigned index );

		const OptDesc& getOptDesc( const std::string& szName ) const;
		const OptDesc& getOptDesc( unsigned index ) const;

		ErrorCode validate( CmdStr& cmdStr ) const;

	private:
		std::vector<OptDesc> m_vOptDesc;
	};

	// For the sake of a consistent interface, this CmdDesc-"factory" is added (Like the 'option' and
	// 'parameter' functions defined above.
	//
	CmdDesc command( const std::string& szName, const std::string& szWhat = "", 
						const std::vector<std::shared_ptr<ParamDesc>>& vpParameters = {},
						const std::vector<OptDesc>& vOptions = {} ) {
		return CmdDesc( szName, szWhat, vpParameters, vOptions );
	}

	//
	// Command Strings (CmdStr)
	// ------------------------
	//
	// Internal data looks like the following table.
	// Assuming a non-empty string is passed to CmdStr constructor,
	// 'getOptionCount()' will always return at least 1, because the
	// command itself is treated as an option.
	// The actual options start at index == 1.
	//
	// vector< pair < string, vector<string> >
	//
	//	Vector     --- Pair ---
	//	Index   First       Second
	//	-----   -----       ------
	//	0       <command>:  <parameter-array0>
	//	1       <option1>:  <parameter-array1>
	//	2       <option2>:  <parameter-array2>
	//	.       .           .
	//	.       .           .
	//
	// Accessing non-existent elements will result in a std::out_of_range
	// exception thrown by the underlying std::vector.
	//
	//
	class CmdStr {
	public:
		// Default constructor.
		CmdStr();

		// Parse command, options and parameters from the string.
		CmdStr( const std::string& szSource );

		// Parse from a stream.
		// Not tested.
		CmdStr( std::istream& isSource );

		// Re-initialize existing object.
		// Returns the number of elements parsed from the source.
		unsigned parseFrom( std::istream& isSource );
		unsigned parseFrom( const std::string& szSource );

		// Used to replace the short hand option name with the full name.
		void adjustOptionName( int optIndex, const std::string& szName );

		// If an empty string is passed to the constructor or parseFrom() function,
		// almost every function below will throw.
		// Before doing anything with the object, it's recommended to check if it's
		// not empty.
		bool isEmpty() const;

		//
		//  Command
		//
		
		const std::string& getCommandName() const;
		int getCommandParameterCount() const;
		const std::string& getCommandParameter( int index ) const;

		//
		//  Option
		//

		// Returns the number of options.
		// If no options are passed to the command, the return value
		// is 1. Please refer to this file's opening comment for
		// insight on the internal data structure.
		int getOptionCount() const;

		// Returns the index of the option. Returns a negative (-1) if the option does not exist.
		// If 'szOptShort' is not set, the function searches for the long version (--option) only.
		// Do not pass the preceding dash(es)!
		int getOptionIndex( const std::string& szOpt, const std::string& szOptShort = std::string("") ) const;

		const std::string& getOptionName( int index );

		//
		//  Parameter
		//
		//  Check boundaries with getOptionIndex() and getParameterCount()
		//

		// Once the option index is retrieved, you can access its parameters.
		int getParameterCount( int optIndex ) const;

		// Get the parameter as string regardless of the intended type.
		const std::string&  getParameter( int optIndex, int paramIndex ) const;

		// Get the parameter converted into 'ParamType'.
		template <typename ParamType>
		ParamType getParameterAs( int optIndex, int paramIndex ) const;

		// Get the whole vector of parameters attached to a single option.
		const std::vector<std::string>& getParameters( int optIndex ) const;

		//
		//  Miscellaneous
		//

		// Print the content of this object in a formatted way.
		// Primarily used as "debugging" helper.
		void print( std::ostream& = std::cout ) const;

	private:
		template <typename T>
		T getParameterAs( int& optIndex, int& paramIndex, const std::true_type& ) const;

		template <typename T>
		T getParameterAs( int& optIndex, int& paramIndex, const std::false_type& ) const;

		std::vector< std::pair<std::string, std::vector<std::string> > > m_vData;
	};


	template <typename T>
	T CmdStr::getParameterAs( int optIndex, int paramIndex ) const {
		return getParameterAs<T>( optIndex, paramIndex, std::is_integral<T>() );
	}


	// Function for integral types
	template <typename T>
	T CmdStr::getParameterAs( int& optIndex, int& paramIndex, const std::true_type& ) const
	{
		auto& optPair{ m_vData.at(optIndex) };
		std::stringstream ss(optPair.second.at(paramIndex));
		T result;
		ss >> std::setbase(0) >> result;
		return result;
	}

	// Function for floating point types, or any other type possible
	template <typename T>
	T CmdStr::getParameterAs( int& optIndex, int& paramIndex, const std::false_type& ) const
	{
		auto& optPair{ m_vData.at(optIndex) };
		std::stringstream ss( optPair.second.at(paramIndex) );
		T result;
		ss >> result;
		return result;
	}

	//
	//
	// Command Descriptor Map (CmdDescMap)
	//
	//
	class CmdDescMap {
	public:

		CmdDescMap( const std::string& szName, bool addHelpDescriptor = true );

		// Adds the built-in "help" descriptor, if there's none in vCmdDesc.
		// If you don't want that, pass 'false' as second argument.
		CmdDescMap( const std::string& szName, const std::vector<CmdDesc>& vCmdDesc, bool addHelpDescriptor = true );

		const std::string& getName() const;

		bool add( const CmdDesc& cmdDesc );

		unsigned getCmdCount() const;

		CmdDesc& operator[]( const std::string& szName );
		CmdDesc& operator[]( unsigned index );

		const CmdDesc& getCmdDesc( const std::string& szName ) const;
		const CmdDesc& getCmdDesc( unsigned index ) const;

		ErrorCode validate( CmdStr& cmdStr ) const;

	private:
		std::vector<CmdDesc> m_vCmdDesc;
		std::string m_szName;
	};



	//
	// Command Map (CmdMap)
	//
	// This class is absolutely optional. It wraps around a std::map, and its sole purpose
	// is to encapsulate the iterator "arithmetics".
	//
	// Template parameter
	// 		T	is supposed to be a callable object/function in this context, but it can be 
	// 			about anything, because the actual call to that object is done outside of 
	// 			this library by the application programmer.
	// 			
	template <class T>
	class CmdMap {
	public:

		CmdMap() = default;
		
		CmdMap( const std::map<std::string,T>& arg ) : m_map{arg} {};

		// Map an object 'ct' to string 'szCmd'. If the key 'szCmd' already exists,
		// the map remains unaltered and false is returned.
		//
		bool add( const std::string& szCmd, T ct ) {
			if (!ct)
				return false;
			auto it = m_map.find(szCmd);
			if ( it != m_map.end() )
				return false;
			m_map[szCmd] = ct;
			return true;
		}

		// Retrieve a copy of the object mapped to 'szCmd'.
		//
		// If not found, default construct and return.
		// A default constructed std::function or a std::shared_ptr
		// evaluates to false, which comes in handy for the purpose
		// of this class.
		//
		T get( const std::string& szCmd ) const {
			for( auto it = m_map.begin(); it != m_map.end(); it++ )
				if ( it->first == szCmd )
					return it->second;
			return T();

			/*
			// Previous implementation using std::map::find()
			//
			auto it = m_map.find(szCmd);    // map<>::find() crashes under some undetermined circumstances.
				if ( it != m_map.end() )
					return it->second;
			return T();       // std::function, std::shared_ptr
			*/
		}


	private:
		std::map<std::string,T> m_map;
	};



	//    Pre-implemented function to print a quick reference of a given CommandDescriptorMap.
	//
	//    Syntax:
	//
	//        help [<command>] [--long|-l]
	//
	//    Examples:
	//
	//        help            - Prints a short syntax reference of all existing commands.
	//        help x          - Prints reference of command "x" only
	//        help --long     - Prints an extended description of all existing commands.
	//        help x --long   - Prints description of command "x" only
	//
	//    Instantiating an appropriate CommandDescriptor:
	//
	//        CommandDescriptor cmdDesc(
	//            "help",
	//            "Print a quick reference of available commands.",
	//            { parameter<Identifier>("command","Print the description of the specified command only.") },
	//            { option("long",'l',"Pass this option to be more descriptive.") }
	//        );
	bool printHelpMessage( const CmdStr& cs, const CmdDescMap& cdm, std::ostream& osDest );
		
} // clp
} // elrat
#endif // include guard

