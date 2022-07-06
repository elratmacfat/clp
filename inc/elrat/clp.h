//
// 	elrat-clp © elratmacfat, 2022
// 
// 	command line processor
//
// 	file: elrat/clp/clp.h
//	
//	What is it?
//	-----------
//	A command within a command line based application consists of the command name itself, its parameters
//	and options which in turn may have own parameters.
//	The command line processor solves the following tasks, typically in the given order:
//		1. The programmer sets up the descriptors and the functions for all of his commands.
//		2. A command line, e.g. "helpme --verbose", is converted into an object, which determines the different
//		parts (command, options and parameters).
//		3. The command object is then processed by the command desciptor map (set up by the programmer during
//		the initialization), which then can tell whether the issued command exists, and if so, also validates
//		the attached options and parameters (Have all mandatory options/parameters been provided? Are there too
//		many? Is the parameter's data type correct?).
//		4. If the command including the options and parameters does match one of the commands described in the
//		command descriptor map, the corresponding function may be invoked with the command object as parameter.
//		The invoked function can safely assume that all the necessary information is provided, and can be 
//		converted into the required types without further type checking.
//	This is a brief summary of what this library can do. The example (src/example.cpp) should help to get a 
//	better idea of how this is achieved in detail.
//
//	Please note that the commands are required the follow the syntax rules defined by this library. For
//	production you'd probably be better off with boost::program_options.
//
//	Command Syntax
//	--------------
//	A command can be followed by zero or more parameters. The last parameter of the command or the previous 
//	option, or previous option's parameter can be followed by another option. Each option can be followed by
//	zero or more parameters.
//
//	Syntax:
//
//		<Command> [<Parameter>]* [<--Option|-O> [<Parameter>]*]*
//
//		< >     Value of command, option or parameter.
//		*       0 or more
//		--      Option name. The double dash is followed by an option name of at least one character.
//		-       Option shorthand. A single dash is followed by exactly one character. The next characters
//				will be treated as the option's parameter, e.g. -d5 is the same as -d 5 or -d=5.
//
//	The inspect_cmdstr application (source file: src/inspect_cmdstr.cpp) is a quick way for you to see
//	how your input command line would be interpreted.
//
//	Background
//	----------
//	The author regularly wrote/writes small programs that require input from the user via the command line.
//	After going through the hassle of implementing the application-specific if-statements over and over 
//	again, the decision to implement a more or less general purpose library for this use case was made.
//
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
	//	Types
	//
	
	typedef enum _ErrorCode {
		SUCCESS = 0,                    // Yeah!
		FAILURE,            // 1        // Something's off, but the author was too lazy to be more communicative about it.
		INVALID_COMMAND,                 // The command name is not existent/could not be found in the map.
		INVALID_OPTION,      // 3        // Option used that is not declared at all (wrong name)
		MISSING_OPTION,                  // Mandatory options are missing.
		TOO_MANY_OPTIONS,     // 5        // More options provided than declared in the descriptor
		INVALID_ARGUMENT,                // Argument is of wrong type or doesn't match the declared constraints, e.g. a number is too high or low.
		MISSING_ARGUMENT,    // 7        // Too few parameters provided. Some parameters may be mandatory.
		TOO_MANY_ARGUMENTS                // Provided arguments may be valid, but there're additional parameters that don't make sense.
	} ErrorCode;

	extern const bool Mandatory;
	extern const bool Optional;

	template <class Callback> class CmdMap;
	template <class Callback> using CommandMap = CmdMap<Callback>;

	class   CmdStr;
	typedef CmdStr      CommandStrings;

	class   CmdDesc;
	typedef CmdDesc     CommandDescriptor;

	class   CmdDescMap;
	typedef CmdDescMap  CommandDescriptorMap;

	class   OptDesc;
	typedef OptDesc     OptionDescriptor;

	class   ParamDesc;
	typedef ParamDesc   ParameterDescriptor;

	class   ParamDescNumN;          typedef ParamDescNumN           NumN;           // N*   (positive numbers + zero)
	class   ParamDescNumZ;          typedef ParamDescNumZ           NumZ;           // Z    (whole numbers)
	class   ParamDescNumR;          typedef ParamDescNumR           NumR;           // R    (real numbers)
	class   ParamDescIdentifier;    typedef ParamDescIdentifier     Identifier;     //
	class   ParamDescString;        typedef ParamDescString         String;         //

	//
	// 	Utility
	//
	
	// Checks if the given string represents number using regular expressions (std::regex)
	bool isHex( const std::string& );	// e.g. 0xAB, 0xf2
	bool isOctal( const std::string& );
	bool isDecimal( const std::string& );	// no leading zeros allowed
	bool isFloatingPoint( const std::string& );


	// Error code to string
	extern const std::map<ErrorCode,const char*> ErrorMessage;



	// 
	// 	Constraint
	//
	//  Constraint objects can be attached to parameter objects.
	//  If a parameter's value doesn't fulfill the constraint's criteria,
	//  the command line is considered invalid. 
	//
	//
	// 	Example:
	//		typedef std::shared_ptr<Constraint> P;
	//
	//  	P p1{ constraint<Min>(5)> };
	//
	//		P p2{ constraint<Max,double>(5.2)> };
	//
	//		P p2{ constraint<Range,double>(5.0,12.4)> };
	
	// Interface
	class Constraint {
	public:
		virtual ErrorCode check( const std::string& szArg ) const = 0;
		virtual std::string toString() const = 0;
	};

	//  Use this template to create constraints
	template <template <class _T> class C, class T, class ... Arg>
	std::shared_ptr<Constraint> constraint( Arg ... a ) {
		return std::shared_ptr<Constraint>( new C<T>( a ... ) );
	}

	// Internal helper (doesnt work with char and unsigned char due to the use of stringstreams)
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
	// 	ParamDesc (Parameter Descriptor)
	//
	//  Usage:
	//
	//  	typedef std::shared_ptr<ParamDesc> P;   // just for the example
	//
	//  	P a{ parameter<Identifier>("name") };
	//  	P b = parameter<NumR>("Radius","Circle's radius, duh.", MANDATORY, { constraint<Min,double>(0) } );
	template<class ParameterType>
	std::shared_ptr<ParamDesc> parameter(
		const std::string& szName,
		const std::string& szWhat = std::string(),
		bool bOptional = Optional,
		const std::vector<std::shared_ptr<Constraint>>& vpConstraints = {})
	{
		return std::shared_ptr<ParamDesc>( new ParameterType(szName,szWhat,bOptional,vpConstraints) );
	}


	class ParamDesc : public Desc {
	public:
		ParamDesc(
			const std::string& szName,
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

	// Numbers: positive whole numbers + zero (N)
	class ParamDescNumN : public ParamDesc {
	public:
		using ParamDesc::ParamDesc; // use non-default constructor of base class
		virtual ErrorCode validate( const std::string& szParam ) const;
	};


	// Numbers: negative and positive whole numbers. (Z)
	class ParamDescNumZ : public ParamDesc {
	public:
		using ParamDesc::ParamDesc;
		virtual ErrorCode validate( const std::string& szParam ) const;
	};

	// Numbers: Floating point number (R)
	class ParamDescNumR : public ParamDesc {
	public:
		using ParamDesc::ParamDesc;
		virtual ErrorCode validate( const std::string& szParam ) const;
	};

	// String: Identifier
	class ParamDescIdentifier : public ParamDesc {
	public:
		using ParamDesc::ParamDesc;
		virtual ErrorCode validate( const std::string& szParam ) const;
	};

	// String: Path
	class ParamDescPath : public ParamDesc {
	public:
		using ParamDesc::ParamDesc;
		virtual ErrorCode validate( const std::string& szParam ) const;
	};

	// String: any String
	class ParamDescString : public ParamDesc {
	public:
		using ParamDesc::ParamDesc;
		virtual ErrorCode validate( const std::string& szParam ) const;
	};


	//
	//	OptDesc (Option Descriptor) 
	//
	//
	
	// For better readability of command descriptor map initialization,
	// use this function to construct an OptDesc.
	OptDesc option(	const std::string& szName,
		const char& szShortName = '\0',
		const std::string& szWhat = std::string(),
		bool bOptional = Optional,
		const std::vector<std::shared_ptr<ParamDesc>>& vpParameters = {} );

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

	// 
	// CmdDesc (Command Descriptor)
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


	//
	// CmdStr (Command String) 
	//
	//	Internal data looks like the following table.
	//	Assuming a non-empty string is passed to CmdStr constructor,
	//	'getOptionCount()' will always return at least 1, because the
	//	command itself is treated as an option.
	//	The actual options start at index == 1.
	//
	//		vector< pair < string, vector<string> >
	//
	//		Vector     --- Pair ---
	//		Index   First       Second
	//		-----   -----       ------
	//		0       <command>:  <parameter-array0>
	//		1       <option1>:  <parameter-array1>
	//		2       <option2>:  <parameter-array2>
	//		.       .           .
	//		.       .           .
	//
	//	Accessing non-existent elements will result in a std::out_of_range
	//	exception thrown by the underlying std::vector.
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
	// CmdDescMap (Command Descriptor Map)
	//
	typedef int CmdIndex;

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
	// CmdMap (Command Map)
	//
	template <class CmdType>
	class CmdMap {
	public:

		CmdMap() { };
		CmdMap( const std::map<std::string,CmdType>& arg ) : m_map{arg} {};

		bool add( const std::string& szCmd, CmdType ct ) {
		if (!ct)
			return false;
		auto it = m_map.find(szCmd);
		if ( it != m_map.end() )
			return false;
			m_map[szCmd] = ct;
		return true;
		}

		CmdType get( const std::string& szCmd ) const {
		for( auto it = m_map.begin(); it != m_map.end(); it++ )
			if ( it->first == szCmd )
			return it->second;
		return CmdType();
		/*
		auto it = m_map.find(szCmd);    // map<>::find() crashes under some undetermined circumstances.
		if ( it != m_map.end() )
			return it->second;
		return CmdType();       // std::function, std::shared_ptr
		*/
		}


	private:
		std::map<std::string,CmdType> m_map;
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
