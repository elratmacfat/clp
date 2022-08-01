//
// project.......: command line processor (clp)
// file..........: inc/elrat/clp.h
// author........: elratmacfat
// description...: user input command line processing library. 
//                 - processing input commands to validate their correctness
//                 in the application context.
//                 - benefits for application programmer are
//                    - within implementation of commands, the command line
//                    is guaranteed to be valid, i.e. satisfy the requirements
//                    defined in the description, that the programmer has to
//                    setup.
//                    - generating user-readable help messages describing the
//                    syntax rules and usage of the commands.
//
// initialization / setup
//
// 1. the programmer describes the application specific commands using
// 	CommandDescriptor and related classes, and adds them to a container 
// 	of type CommandDescriptorMap.
// 	
// 2. the programmer implements the command functions and maps them to their 
// 	command names (that are also used in the command description).
// 	The map to the implemented command to the command name, a CommandMap 
// 	can be used.
// 
// in main / loop over input source / ...
//
// 3. an input command line, e.g. something like "help me -i --am_lost", has 
// 	to be passed to an elrat::clp::CommandStrings object, which parses
// 	the elements of the command.
// 	
// 	TODO In the current the state the syntax rules are constant and defined 
// 	by the library, which reduces usability of the library and should be
// 	addressed soon.
//
// 4. the CommandStrings object then has to be passed to the
// 	CommandDescriptorMap, that was defined in step 1, which compares the
// 	input command with the descriptions.
//
// 5. if the issued command is considered valid, the corresponding command
// 	function can be retrieved from the CommandMap, defined in step 2.
//
// 6. invoke the command which should take the CommandStrings object as 
// 	parameter.
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

// ErrorCode is used to give feedback about issued commands
//
typedef enum _ErrorCode {
	SUCCESS = 0,
	FAILURE,
	INVALID_COMMAND,
	INVALID_OPTION, 
	MISSING_OPTION, 
	TOO_MANY_OPTIONS,
	INVALID_ARGUMENT,
	MISSING_ARGUMENT,
	TOO_MANY_ARGUMENTS
} ErrorCode;

// When setting up your command descriptors, you have to
// specify if a parameteris optional or mandatory. Instead
// of passing a bland 'true' or 'false', you can use these
// constants, making the call more verbose about what's 
// going on.
//
extern const bool Mandatory; // false
extern const bool Optional;  // true

// The CmdMap/CommandMap is a std::map with a std::string as
// key (the command name) and a 'Callback' as value.  The 
// 'Callback' can actually be anything. It doesn't even have
// to be callable. But it should be, if you follow the intended
// usage (as shown in src/example.cpp)
//
template <class Callback> class CmdMap;
template <class Callback> using CommandMap = CmdMap<Callback>;

// CmdStr/CommandStrings is the object that takes a string, 
// e.g. "mycommand 1337 --verbose", and parses/separates
// parameters and options.
//
class CmdStr;
using CommandStrings = CmdStr;

// A command descriptor is an aggregate of options and 
// parameter descriptors.
//
class CmdDesc;
using CommandDescriptor = CmdDesc;

// The complete description of a command is mapped to its own
// name, making it easy and fast to find the corresponding
// description in a typical use case (The user issues a command
// line, which first 'word' is the command itself).
//
class CmdDescMap;
using CommandDescriptorMap = CmdDescMap;

// Option Descriptor. Is very similar to a Command Descriptor,
// but may be declared mandatory or optional (I see the paradox
// in the nomenclature, but a mandatory option is possible just
// as 'git commit' sort of requires '-m').
// TODO Review design decision, wether an option can be mandatory.
//
class OptDesc;
using OptionDescriptor = OptDesc;

// Each of the following Parameter Descriptors imply their 
// intended usage and datatype.
//

// Parameter Descriptor Interface
class ParamDesc;

// N = Natural number (including zero).
class ParamDescNumN;          
using NumN = ParamDescNumN; 

// Z = Whole numbers (negative, zero, positive)
class ParamDescNumZ;          
using NumZ = ParamDescNumZ;

// R = Real numbers (such Pi)
class ParamDescNumR;          
using NumR = ParamDescNumR;          

class ParamDescIdentifier;    
using Identifier = ParamDescIdentifier;

class ParamDescString;        
using String = ParamDescString;

// Interface for constraints that can be attached to parameters.
class Constraint;

// 
using ConstraintPtr = std::shared_ptr<Constraint>;
using ParamDescPtr = std::shared_ptr<ParamDesc>;
// Check if the given string represents a number using regular
// expressions (std::regex)
//

// e.g. 0xAB, 0xf2
bool isHex( const std::string& ); 

bool isOctal( const std::string& );

// No leading zeros allowed, because this would indicate an
// octal representation.
bool isDecimal( const std::string& ); 

// Any decimal can be interpreted as a floating point.
bool isFloatingPoint( const std::string& );

//  
//
std::string toString( const ErrorCode& );

//
//
std::ostream& operator<<( std::ostream&, const ErrorCode& );

// Constraint interface
//
// While Parameter Descriptor defines its required data *type*.
// a 'Constraint' added to a parameter, defines the range of 
// valid values.
//
// Example
//  	auto p1{ constraint<Min>(5)> };
//	auto p2{ constraint<Max,double>(5.2)> };
//	auto p2{ constraint<Range,double>(5.0,12.4)> };
//
class Constraint {
public:
	virtual ErrorCode check( const std::string& szArg ) const = 0;
	virtual std::string toString() const = 0;
};

// Parameters:
// T_	template parameter of C (same as T).
// C	specified 'Constraint', namely one of 'Min', 'Max' or 'Range'.
// T	data type of the constraint itself and therefore also the  
//	type of function arguments (constraint values).
// Arg	The constraint parameters
//
template <template <class T_> class C, class T, class ... Arg>
ConstraintPtr constraint( Arg ... a ) {
	return ConstraintPtr( new C<T>( a ... ) );
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
		throw std::runtime_error("convert(): invalid argument.");
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
		return ( t >= m_min && t <= m_max ) 
			? SUCCESS 
			: INVALID_ARGUMENT;
	}

	virtual std::string toString() const {
		return std::to_string(m_min) 
			+ std::string("...")
			+ std::to_string(m_max);
	}
private:
	NumType m_min, m_max;
};

// Descriptor (Desc) 
//
// Base class for command, option and parameter. 
// Provides a 'name' and a descriptive 'what' line.
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

// Parameter Descriptor (ParamDesc)
// 
// Parameters are attached to Command and Option Descriptors.
// Each parameters can have a set of constraints.
//
// The following is the base class for specified Parameter 
// Descriptors. The individual specifications imply their data
// type and to some extent their intended usage.
//
class ParamDesc : public Desc {
public:
	ParamDesc( const std::string& szName,
		const std::string& szWhat = std::string(""),
		bool bOptional = Optional,
		const std::vector<ConstraintPtr>& 
			vpConstraints = {});

	virtual ~ParamDesc();

	bool isOptional() const;

	const std::vector<ConstraintPtr>& getConstraints() const;

	virtual ErrorCode validate( const std::string& szParam ) const = 0;

protected:
	bool m_bOptional;
	std::vector<ConstraintPtr> m_vpConstraints;
};

// Use this template to create parameters for better readability. 
// As opposed to typing out the std::shared_ptr<> stuff.
//
// Example:
//  	auto p = parameter<NumR>("width", 'w', "Rectangle's width");
//	
template<class ParameterType>
ParamDescPtr parameter( const std::string& szName,
	const std::string& szWhat = std::string(),
	bool bOptional = Optional,
	const std::vector<ConstraintPtr>& vpConstraints = {})
{
	return ParamDescPtr( 
		new ParameterType(szName,szWhat,bOptional,vpConstraints));
}

// Numbers: positive whole numbers + zero (N)
//
class ParamDescNumN : public ParamDesc {
public:
	using ParamDesc::ParamDesc; // = Use non-default ctor of base class
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

// String: Identifier 
// like variable names and such, no special characters allowed
//
class ParamDescIdentifier : public ParamDesc {
public:
	using ParamDesc::ParamDesc;
	virtual ErrorCode validate( const std::string& szParam ) const;
};

// String: Path
// TODO Go to src/clp.cpp and implement the regular expression for the
// 	ParamDescPath. Not implemented and not tested. Totally neglected.
class ParamDescPath : public ParamDesc {
public:
	using ParamDesc::ParamDesc;
	virtual ErrorCode validate( const std::string& szParam ) const;
};

// String: any kind of string, likes text messages.
//
class ParamDescString : public ParamDesc {
public:
	using ParamDesc::ParamDesc;
	virtual ErrorCode validate( const std::string& szParam ) const;
};

// Option Descriptor (OptDesc)
//
class OptDesc : public Desc {
public:
	OptDesc( const std::string& szName,
		const char& szShortName,
		const std::string& szWhat,
		bool bOptional = Optional,
		const std::vector<ParamDescPtr>& 
			vpParameters = {} );

	bool add( const ParamDescPtr& pParamDesc );

	unsigned getParamCount() const;
	
	bool isOptional() const;
	
	std::string getShortName() const;

	ParamDescPtr& operator[]( const std::string& szName );
	
	ParamDescPtr& operator[]( unsigned index );

	const ParamDescPtr& 
		getParamDesc( const std::string& szName ) const;
	
	const ParamDescPtr& 
		getParamDesc( unsigned index ) const;

	ErrorCode validate(const std::vector<std::string>& vParameters) const;

private:
	const char m_cName;
	const bool m_bOptional;
	std::vector<ParamDescPtr> m_vpParamDesc;
};

// For better readability of the Command Descriptor's initialization,
// use this function to construct an Option Descriptor.
// 
// Example
//	option("message",'m',"Meaningful description of what you've changed.", 
//		Mandatory, 
//		{
// 			parameter<String>("msg","commit message")
// 		}
// 	);
//
OptDesc option(	const std::string& szName,
	const char& szShortName = '\0',
	const std::string& szWhat = std::string(),
	bool bOptional = Optional,
	const std::vector<ParamDescPtr>& vpParameters = {} );

// Command Descriptor (CmdDesc)
//
// Example:
//
//	CmdDesc cd = command("dump", "Dump the specified state.", 
//		{
// 		parameter<NumN>("state","State ID", Optional )
// 		},{
// 		option("file",'f',"Write it to a file.", Optional, 
// 			{
// 			parameter<String>("filename", 
// 				"Destination file.", 
// 				Mandatory )
// 			})
// 		}
// 	);
//
class CmdDesc : public Desc {
public:
	CmdDesc( const std::string& szName,
		const std::string& szWhat = "",
		const std::vector<ParamDescPtr>& 
			vpParameters = {},
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

// To provide a consistent interface this function 
// is introduced, doing the constructor call. 
//
CmdDesc command( const std::string& szName, const std::string& szWhat = "", 
	const std::vector<ParamDescPtr>& vpParameters = {},
	const std::vector<OptDesc>& vOptions = {} ) 
{
	return CmdDesc( szName, szWhat, vpParameters, vOptions );
}

// Command Strings (CmdStr)
//
// Internal data looks like the following table.
// Assuming a non-empty string is passed to CmdStr constructor,
// 'getOptionCount()' will always return at least 1, because the
// command itself is treated as an option.
// The actual options start at index == 1.
//
// vector< pair < string, vector<string> >
//
//Vector     --- Pair ---
//Index   First       Second
//-----   -----       ------
//0       <command>:  <parameter-array0>
//1       <option1>:  <parameter-array1>
//2       <option2>:  <parameter-array2>
//.       .           .
//.       .           .
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

	// Parse from a stream. Not tested.
	CmdStr( std::istream& isSource );

	// Re-initialize existing object. Returns the number
	// of elements parsed from the source.
	unsigned parseFrom( std::istream& isSource );
	unsigned parseFrom( const std::string& szSource );

	// Used to replace the short hand option name with 
	// the full name.
	void adjustOptionName( int optIndex, const std::string& szName );

	// If an empty string is passed to the constructor or
	// parseFrom() function, almost every function below will
	// throw. Before doing anything with the object, it's
	// recommended to check if it's not empty.
	bool isEmpty() const;

	//
	const std::string& getCommandName() const;
	
	//
	int getCommandParameterCount() const;
	
	//
	const std::string& getCommandParameter( int index ) const;

	// Returns the number of options.
	// If no options are passed to the command, the return value
	// is 1. Please refer to this file's opening comment for
	// insight on the internal data structure.
	int getOptionCount() const;


	// Returns the index of the option. Returns a negative (-1) 
	// if the option does not exist. If 'szOptShort' is not set, 
	// the function searches for the long version (--option) only.
	// Do not pass the preceding dash(es)!
	int getOptionIndex( const std::string& szOpt, 
		const std::string& szOptShort = std::string("") ) const;

	//
	const std::string& getOptionName( int index );

	// Once the option index is retrieved, you can access its parameters.
	int getParameterCount( int optIndex ) const;

	// Get the parameter as string regardless of the intended type.
	const std::string&  getParameter( int optIndex, int paramIndex ) const;

	// Get the parameter converted into 'ParamType'.
	template <typename ParamType>
	ParamType getParameterAs( int optIndex, int paramIndex ) const;

	// Get the whole vector of parameters attached to a single option.
	const std::vector<std::string>& getParameters( int optIndex ) const;

	// Print the content of this object in a formatted way.
	// Primarily used as "debugging" helper.
	void print( std::ostream& = std::cout ) const;

private:
	template <typename T>
	T getParameterAs(int& optIndex, 
		int& paramIndex, 
		const std::true_type& ) const;

	template <typename T>
	T getParameterAs( int& optIndex, 
		int& paramIndex, 
		const std::false_type& ) const;

	std::vector<std::pair<std::string, std::vector<std::string>>> m_vData;
};


template <typename T>
T CmdStr::getParameterAs( int optIndex, int paramIndex ) const {
	return getParameterAs<T>(optIndex, paramIndex, std::is_integral<T>());
}


// Function for integral types
template <typename T>
T CmdStr::getParameterAs( 
	int& optIndex, 
	int& paramIndex, 
	const std::true_type& ) const
{
	auto& optPair{ m_vData.at(optIndex) };
	std::stringstream ss(optPair.second.at(paramIndex));
	T result;
	ss >> std::setbase(0) >> result;
	return result;
}

// Function for floating point types, or any other type possible
template <typename T>
T CmdStr::getParameterAs( 
	int& optIndex, 
	int& paramIndex, 
	const std::false_type& ) const
{
	auto& optPair{ m_vData.at(optIndex) };
	std::stringstream ss( optPair.second.at(paramIndex) );
	T result;
	ss >> result;
	return result;
}

// Command Descriptor Map (CmdDescMap)
//
class CmdDescMap {
public:

	//
	CmdDescMap( const std::string& szName, bool addHelpDescriptor = true );

	// Adds the built-in "help" descriptor, if there's
	// none found in the argument 'vCmdDesc'.
	// If you don't want that, pass 'false' as last argument.
	CmdDescMap( const std::string& szName, 
		const std::vector<CmdDesc>& vCmdDesc, 
		bool addHelpDescriptor = true );

	//
	const std::string& getName() const;

	//
	bool add( const CmdDesc& cmdDesc );

	//
	unsigned getCmdCount() const;

	//
	CmdDesc& operator[]( const std::string& szName );

	//
	CmdDesc& operator[]( unsigned index );

	//
	const CmdDesc& getCmdDesc( const std::string& szName ) const;

	//
	const CmdDesc& getCmdDesc( unsigned index ) const;

	//
	ErrorCode validate( CmdStr& cmdStr ) const;

private:
	std::vector<CmdDesc> m_vCmdDesc;
	std::string m_szName;
};



// Command Map (CmdMap)
//
// This class is absolutely optional. It wraps around a 
// std::map, and its sole purpose is to encapsulate the 
// iterator "arithmetics".
//
// template parameter
// T	is supposed to be a callable object/function in
// 	this context, but it can be about anything,
// 	because the actual call to that object is done 
// 	outside of this library by the application
// 	programmer.
// 			
template <class T>
class CmdMap {
public:
	//
	CmdMap() = default;
	
	//
	CmdMap( const std::map<std::string,T>& arg ) : m_map{arg} {};

	// Map an object 'ct' to string 'szCmd'. If the key 'szCmd' already 
	// exists, the map remains unaltered and false is returned.
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
	}
private:
	std::map<std::string,T> m_map;
};



// Pre-implemented function to print a quick reference of a given 
// CommandDescriptorMap.
//
// Syntax:
//	help [<command>] [--long|-l]
//
// Examples:
//	help            - Prints a short syntax reference of all commands.
//	help x          - Prints reference of command "x" only
//	help --long     - Prints an extended description of all commands.
//	help x --long   - Prints description of command "x" only
//
void printHelpMessage( 
	const CmdStr& cs, 
	const CmdDescMap& cdm, 
	std::ostream& osDest );
	
} // clp
} // elrat
#endif // include guard

