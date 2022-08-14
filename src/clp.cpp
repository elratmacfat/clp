//
// project.......: command line processor (clp)
// file..........: src/clp.cpp
// author........: elratmacfat
// description...: implementation of inc/elrat/clp.h:
//
#include <cassert>
#include <iostream>
#include <memory>
#include <regex>

#include <elrat/clp.hpp>

// Runtime assert with message.
//
#define assertm(exp, msg) assert(((void)msg, exp)) // cppreference.com

namespace elrat {
namespace clp {

//
//
// Types & Constants
//
//

// Be more verbose and use these constants as function arguments
// for parameter and option definitions (instead of passing just 
// true/false).
const bool Mandatory{false};
const bool Optional{true};

// This command descriptor is used as default help-command in any command 
// descriptor map.
//
const CmdDesc cmdDescHelp( "help", 
	"Print a quick reference of available commands.", 
	{
		parameter<Identifier>("command",
			"Print the description of the specified command only.") 
	},
	{ 
		option("long",'l',
			"Pass this option to be more descriptive.") 
	}
);

//
//
static const std::map<ErrorCode,const char*> ErrorMessages{
	 {SUCCESS,		"Success."}
	,{FAILURE,		"Undefined error."}
	,{INVALID_COMMAND,	"Command not found (in the current context)."}
	,{INVALID_OPTION,	"Invalid option."}
	,{MISSING_OPTION,	"Missing option."}
	,{TOO_MANY_OPTIONS,	"Too many options provided."}
	,{INVALID_ARGUMENT,	"Invalid argument."}
	,{MISSING_ARGUMENT,	"Missing argument."}
	,{TOO_MANY_ARGUMENTS, 	"Too many arguments provided."}
};

//
//
//	Utility
//
//

static bool is_X ( const std::string& szArg, const std::string& szRegex )
{
	return std::regex_match(szArg,std::regex(szRegex));
}

bool isHex( const std::string& szArg )
{
	return is_X( szArg, "^(0[Xx][0-9a-fA-F]+)$" );
}

bool isOctal( const std::string& szArg )
{
	return is_X( szArg, "^(0[0-7]+)$" );
}

bool isDecimal( const std::string& szArg )
{
	return is_X( szArg, "(^([+-]?[1-9]\\d+)$)|(^([+-]?0+)$)" );
}

bool isFloatingPoint( const std::string& szArg)
{
	return is_X( szArg, "^([+-]?\\d*.?\\d+)$");
}

// Converts the given error code into a human readable error message.
//
std::string toString( const ErrorCode& ec ) {
	try {
		return ErrorMessages.at( ec );
	} catch( std::out_of_range ) {
	}
	return std::string("Unknown error code.");
}

//
//
std::ostream& operator<<( std::ostream& os, const ErrorCode& ec ) {
	os << toString( ec );
	return os;
}

//
// 
//	ParameterDescriptor (ParamDesc)
//
//

ErrorCode internal_validate( 
	const std::string& szRegex, 
	const std::string& szParam ) 
{
	return ( regex_match( szParam, std::regex( szRegex ) )
			? ErrorCode::SUCCESS
			: ErrorCode::INVALID_ARGUMENT );
}

ErrorCode internal_constraints_check( 
	const std::vector<ConstraintPtr>& vpConstraints, 
	const std::string& szParam ) 
{
	for( auto& pConstraint : vpConstraints ) {
		ErrorCode result {pConstraint->check(szParam)};
		if ( result != SUCCESS )
			return result;
	}
	return SUCCESS;
}

ParamDesc::ParamDesc(
	const std::string& szName,
	const std::string& szWhat,
	bool bOptional,
	const std::vector<ConstraintPtr>& vpConstraints )
: Desc( szName, szWhat ),
m_bOptional{bOptional},
m_vpConstraints(vpConstraints)
{

}

ParamDesc::~ParamDesc() {

}

bool ParamDesc::isOptional() const
{
	return m_bOptional;
}

const std::vector<ConstraintPtr>& ParamDesc::getConstraints() const
{
	return m_vpConstraints;
}

ErrorCode ParamDescNumN::validate( const std::string& szParam ) const {
	ErrorCode result{ internal_validate(
		"(^(\\d+)$)|(^(0x[0-9a-fA-F]+)$)|(^(0[0-7]+)$)", 
		szParam ) };
	if ( result != SUCCESS)
		return result;
	return internal_constraints_check( 
		ParamDesc::m_vpConstraints, 
		szParam );
}

ErrorCode ParamDescNumZ::validate( const std::string& szParam ) const {
	ErrorCode result{ internal_validate(
		"^([+-]?\\d+)$", 
		szParam ) };
	if ( result != SUCCESS)
		return result;
	return internal_constraints_check( 
		ParamDesc::m_vpConstraints, 
		szParam );
}

ErrorCode ParamDescNumR::validate( const std::string& szParam ) const {
	ErrorCode result{ internal_validate(
		"^([+-]?\\d*.?\\d+)$", 
		szParam ) };
	if ( result != SUCCESS)
		return result;
	return internal_constraints_check( 
		ParamDesc::m_vpConstraints, 
		szParam );
}

ErrorCode ParamDescIdentifier::validate( const std::string& szParam ) const {
	ErrorCode result{ internal_validate(
		"^([_a-zA-Z][_\\w]*)$", 
		szParam ) };
	if ( result != SUCCESS)
		return result;
	return internal_constraints_check( 
		ParamDesc::m_vpConstraints, 
		szParam );
}

// TODO Find the correct regular expression for 'path'. What about the 
// 	differences between Windows/MacOS/Linux?
ErrorCode ParamDescPath::validate( const std::string& szParam ) const {
	ErrorCode result{ internal_validate(
		"^([_\\w]+)$",  // placeholder
		szParam ) };
	if ( result != SUCCESS)
		return result;
	return internal_constraints_check( 
		ParamDesc::m_vpConstraints, 
		szParam );
}

ErrorCode ParamDescString::validate( const std::string& szParam ) const {
	ErrorCode result{ internal_validate(
		"^(.+)$", 
		szParam ) };
	if ( result != SUCCESS)
		return result;
	return internal_constraints_check( 
		ParamDesc::m_vpConstraints, 
		szParam );
}

//
//
// OptionDescriptor (OptDesc) 
//
//

OptDesc option(
	const std::string& szName,
	const char& szShortName,
	const std::string& szWhat,
	bool bOptional,
	const std::vector<ParamDescPtr>& vpParameters)
{
	return OptDesc(szName,szShortName,szWhat,bOptional,vpParameters);
}

OptDesc::OptDesc(
	const std::string& szName,
	const char& cName,
	const std::string& szWhat,
	bool bOptional,
	const std::vector<ParamDescPtr>& vpParamDesc )
: Desc(szName,szWhat), 
m_cName{cName}, 
m_bOptional{bOptional}, 
m_vpParamDesc{vpParamDesc}
{
	for( unsigned i{1}; i < m_vpParamDesc.size(); i++ )
		assertm(
		   !(!m_vpParamDesc[i]->isOptional() && 
		   m_vpParamDesc[i-1]->isOptional()),
		   "Mandatory parameters must be declared before"
		   	"any optional parameters.");
}

bool OptDesc::add( const ParamDescPtr& pParamDesc )
{
	if ( !pParamDesc )
		return false;
	for( const ParamDescPtr& p : m_vpParamDesc )
		if ( p->getName() == pParamDesc->getName() )
			return false;
	m_vpParamDesc.push_back( pParamDesc );

	unsigned long long n{ m_vpParamDesc.size() - 1 };
	if ( n )
		assertm(
		   !(!m_vpParamDesc[n]->isOptional() && 
		   m_vpParamDesc[n-1]->isOptional()),
		   "Mandatory parameters must be declared before"
		   	"any optional parameters.");

	return true;
}

unsigned OptDesc::getParamCount() const
{
	return m_vpParamDesc.size();
}

bool OptDesc::isOptional() const
{
	return m_bOptional;
}

std::string OptDesc::getShortName() const {
	if ( m_cName )
		return std::string(1,m_cName);
	else
		return std::string();
}

ParamDescPtr& OptDesc::operator[]( const std::string& szName )
{
	for ( ParamDescPtr& p : m_vpParamDesc )
		if ( p->getName() == szName )
			return p;
	throw std::out_of_range(
		"ParamDescPtr& OptDesc::getParamDesc(const std::string&)");
}

ParamDescPtr& OptDesc::operator[]( unsigned index )
{
	if ( index < m_vpParamDesc.size() )
		return m_vpParamDesc[index];
	throw std::out_of_range(
		"ParamDescPtr& OptDesc::getParamDesc(unsigned)");
}

const ParamDescPtr& OptDesc::getParamDesc( const std::string& szName ) const
{
	for ( const ParamDescPtr& p : m_vpParamDesc )
		if ( p->getName() == szName )
			return p;
	throw std::out_of_range(
		"const ParamDescPtr& OptDesc::getParamDesc(const "
			"std::string&) const");
}

const ParamDescPtr& OptDesc::getParamDesc( unsigned index ) const
{
	if ( index < m_vpParamDesc.size() )
		return m_vpParamDesc[index];
	throw std::out_of_range(
		"const ParamDescPtr& OptDesc::getParamDesc(unsigned) const");
}

ErrorCode OptDesc::validate( const std::vector<std::string>& vs ) const {
	ErrorCode result{SUCCESS};
	unsigned i{0};
	unsigned mandatory{0};
	for( auto& pParamDesc : m_vpParamDesc )
		if ( !pParamDesc->isOptional() )
			mandatory++;
	if ( vs.size() > m_vpParamDesc.size() )
		return ErrorCode::TOO_MANY_ARGUMENTS;
	for( ; i < vs.size(); i++ ) {
		result = m_vpParamDesc[i]->validate( vs[i] );
		if ( result != SUCCESS )
			return result;
	}
	if ( i < mandatory )
		return ErrorCode::MISSING_ARGUMENT;
	return result;
}


//
//
//	CommandDescriptor (CmdDesc)
//
//

CmdDesc::CmdDesc(
		const std::string& szName,
		const std::string& szWhat,
		const std::vector<ParamDescPtr>& vpParamDesc,
		const std::vector<OptDesc>& vOptDesc)
:	Desc(szName,szWhat)
{
	// Because the command itself does have <_parameters> just like the
	// <OptDesc> does, we treat the command and its parameters as the 
	// first option. The commands' <_parameters> go to vOptDesc[0]. The 
	// passed <_options> start at vOptDesc[1].
	//
	// This is a bit inconvenient, but saves me from writing the same code
	// twice.

	m_vOptDesc.push_back( OptDesc( szName, '\0', szWhat, Mandatory ) );
	for ( auto& p : vpParamDesc )
		m_vOptDesc[0].add( p );

	for( const OptDesc& optDesc : vOptDesc )
		m_vOptDesc.push_back( std::move(optDesc) );


	for( unsigned i{1}; i < m_vOptDesc.size(); i++ )
		assertm(
		   !(!m_vOptDesc[i].isOptional() && 
		   m_vOptDesc[i-1].isOptional()),
		   "Mandatory options must be declared before "
		   	"any optional options.");
}

bool CmdDesc::add( const OptDesc& optDesc )
{
	for( const OptDesc& obj : m_vOptDesc )
		if ( obj.getName() == optDesc.getName() ||
			obj.getShortName() == optDesc.getName() ||
			obj.getName() == optDesc.getShortName() )
			return false;
	m_vOptDesc.push_back( optDesc );
	unsigned long long n{ m_vOptDesc.size() - 1 };
	if ( n )
		assertm(
		   !(!m_vOptDesc[n].isOptional() && 
		   m_vOptDesc[n-1].isOptional()),
		   "Mandatory options must be declared before "
		   	"any optional options.");
	return true;
}

unsigned CmdDesc::getOptCount() const
{
	return m_vOptDesc.size();
}

OptDesc& CmdDesc::operator[]( const std::string& szName )
{
	for( OptDesc& optDesc : m_vOptDesc )
		if ( optDesc.getName() == szName )
			return optDesc;
	throw std::out_of_range(
		"OptDesc& CmdDesc::operator[](const std::string&)");
}

OptDesc& CmdDesc::operator[]( unsigned index )
{
	if ( index < m_vOptDesc.size() )
		return m_vOptDesc[index];
	throw std::out_of_range("OptDesc& CmdDesc::operator[](unsigned)");
}

const OptDesc& CmdDesc::getOptDesc( const std::string& szName ) const
{
	for( const OptDesc& optDesc : m_vOptDesc )
		if ( optDesc.getName() == szName )
			return optDesc;
	throw std::out_of_range("const OptDesc& CmdDesc::getOptDesc(const "
		"std::string&) const");
}

const OptDesc& CmdDesc::getOptDesc( unsigned index ) const
{
	if ( index < m_vOptDesc.size() )
		return m_vOptDesc[index];
	throw std::out_of_range(
		"const OptDesc& CmdDesc::getOptDesc(unsigned) const");
}

ErrorCode CmdDesc::validate( CmdStr& cmdStr ) const
{
	ErrorCode result{INVALID_OPTION};

	// Check if this <cmdStr> matches this <CmdDesc> in general by
	// comparing the command string (the first word of the input line).
	if ( cmdStr.isEmpty() || (cmdStr.getCommandName() !=  Desc::m_szName) )
		return ErrorCode::INVALID_COMMAND;

	// Are too many options passed?
	if ( cmdStr.getOptionCount() > static_cast<int>(m_vOptDesc.size()) )
		return ErrorCode::TOO_MANY_OPTIONS;

	// Check if the bOptionaluired amount of options have been passed to
	// the command.
	int mandatory{0};
	for ( unsigned i{1}; i < m_vOptDesc.size(); i++ )
		if ( !m_vOptDesc[i].isOptional() )
			mandatory++;
	if ( cmdStr.getOptionCount() < mandatory )
		return ErrorCode::MISSING_OPTION;

	// Let each <OptDesc> check if they find themselves satisfied 
	// in <CmdStr>
	// covered cases are: 
	// 	INVALID_OPTION, 
	// 	TOOMANY_OPT, 
	// 	INVALID_ARG, 
	// 	TOOMANY_ARG
	for ( const OptDesc& optDesc : m_vOptDesc ) {
		int optIndex{ cmdStr.getOptionIndex( 
			optDesc.getName(), 
			optDesc.getShortName()) };
		if ( optIndex >= 0 ) {
			// The option may have been passed by its shorthand 
			// form. Overwrite to make sure every option in the 
			// CmdStr-object is given in its long version.
			cmdStr.adjustOptionName(optIndex, optDesc.getName());
			// Pass the CmdStr option's parameters to the Option 
			// Descriptor to let it validate them.
			auto& parameters{ cmdStr.getParameters(optIndex) };
			result = optDesc.validate( parameters );
			if ( result != SUCCESS )
				return result;
		}
	}

	// Check the other way round
	// See if there're options provided, that do not match any of the 
	// Command Descriptor's
	int cmdStrOptCount{ cmdStr.getOptionCount() };
	for( int i{0}; i < cmdStrOptCount; i++ ) {
		bool found{false};
		for( const OptDesc& optDesc : m_vOptDesc ) {
			const std::string& szOpt{ cmdStr.getOptionName(i) };
			if ( szOpt == optDesc.getName() || 
				szOpt == optDesc.getShortName() ) 
			{
				found=true;
				break;
			}
		}
		if (!found)
			return INVALID_OPTION;
	}
	return result;
}

//
// 	CommandStrings (CmdStr)
//
CmdStr::CmdStr()
{
	//m_vData.clear();
}

CmdStr::CmdStr( const std::string& szSource )
{
	parseFrom( szSource );
}

CmdStr::CmdStr( std::istream& isSource )
{
	parseFrom( isSource );
}


unsigned CmdStr::parseFrom( const std::string& szSource )
{
	m_vData.clear();
	if ( !szSource.length() )
		return 0;
	std::vector<std::string> vElements;
	std::regex separating_regex("(\".+\")|(-[a-zA-Z])|([^= \\s]+)");
	auto itBegin = std::sregex_iterator( 
		szSource.begin(), 
		szSource.end(), 
		separating_regex );
	auto itEnd = std::sregex_iterator();
	for ( auto it = itBegin; it != itEnd; it++ )
		vElements.push_back( it->str() );
	unsigned iOption{0};
	m_vData.push_back( 
		std::make_pair( vElements[0], std::vector<std::string>() ));
	for( unsigned i{1}; i < vElements.size(); i++ ) {

		bool isOption{false};

		// If the first character is a '-', determine if it's
		// a hyphen preceeding an option, or a minus preceeding
		// numeric parameter.
		if ( vElements[i][0] == '-' ) {
			std::regex negative("^(-\\d*.?\\d+)$");
			isOption = !std::regex_match( vElements[i], negative );
		}

		// Remove the hyphen if it's an option
		while ( isOption && vElements[i][0] == '-' )
			vElements[i].erase( 0, 1 );

		// Add element as option or parameter.
		if ( isOption ) {
			m_vData.push_back( 
				std::make_pair( 
					vElements[i], 
					std::vector<std::string>() ));
			iOption++;
		}
		else {
			m_vData[iOption].second.push_back( vElements[i] );
		}
	}
	return vElements.size();
}

unsigned CmdStr::parseFrom( std::istream& isSource )
{
	std::string inputLine;
	std::getline( isSource, inputLine );
	return parseFrom( inputLine );
}

void CmdStr::adjustOptionName( int optIndex, const std::string& szName )
{
	m_vData[optIndex].first = szName;
}

bool CmdStr::isEmpty() const {
	return (!m_vData.size());
}

//
//	CmdStr: Command related
//

const std::string& CmdStr::getCommandName() const {
	return m_vData.at(0).first;
}

int CmdStr::getCommandParameterCount() const {
	auto& cmdPair{ m_vData.at(0) };
	return cmdPair.second.size();
}

const std::string& CmdStr::getCommandParameter( int index ) const {
	auto& cmdPair{ m_vData.at(0) };
	return cmdPair.second[index];
}


//
//	CmdStr: Option related
//

int CmdStr::getOptionCount() const
{
	return m_vData.size();
}

int CmdStr::getOptionIndex( 
	const std::string& szOpt, 
	const std::string& szShort ) const
{
	int iSize{ static_cast<int>(m_vData.size()) };
	for( int index{0}; index < iSize ; index++ ) {
		if ( m_vData[index].first == szOpt || 
			m_vData[index].first == szShort )
		{
			return index;
		}
	}
	return -1;
}

const std::string& CmdStr::getOptionName( int index )
{
	auto& optPair{m_vData[index]};
	return optPair.first;
}

//
//	CmdStr: Parameter related
//

int CmdStr::getParameterCount( int optIndex ) const
{
	auto& optPair{ m_vData.at(optIndex) };
	return optPair.second.size();
}

const std::string& CmdStr::getParameter( int optIndex, int paramIndex ) const {
	auto& optPair{ m_vData.at(optIndex) };
	return optPair.second.at(paramIndex);
}

const std::vector<std::string>& CmdStr::getParameters( int optIndex ) const {
	auto& optPair{ m_vData.at(optIndex) };
	return optPair.second;
}


//
//	CmdStr: Miscellaneous
//

void CmdStr::print( std::ostream& osDest  ) const
{
	for( auto& o : m_vData ) {
		osDest << "['" << o.first << "':<";
		for( unsigned i{0}; i < o.second.size(); i++ ) {
			osDest << "'" << o.second[i] << "'";
			if ( (i+1)<o.second.size() ) osDest << ",";
		}
		osDest << ">]\n";
	}
}

//
//
//	CommandDescriptorMap (CmdDescMap)
//
//

CmdDescMap::CmdDescMap( const std::string& szName, bool addHelpDescriptor )
: m_szName{szName}
{
	if ( addHelpDescriptor ) {
	   add( cmdDescHelp );
	}
}

CmdDescMap::CmdDescMap( 
	const std::string& szName, 
	const std::vector<CmdDesc>& vCmdDesc, 
	bool addHelpDescriptor )
: m_szName{szName}
{
	if ( addHelpDescriptor )
		add( cmdDescHelp );
	for( auto c : vCmdDesc )
		add( c );
}

const std::string& CmdDescMap::getName() const
{
	return m_szName;
}

bool CmdDescMap::add( const CmdDesc& cmdDesc )
{
	for ( const CmdDesc& obj : m_vCmdDesc )
		if ( obj.getName() == cmdDesc.getName() )
			return false;
	m_vCmdDesc.push_back( cmdDesc );
	return true;
}

unsigned CmdDescMap::getCmdCount() const
{
	return m_vCmdDesc.size();
}

CmdDesc& CmdDescMap::operator[](const std::string& szName )
{
	for( CmdDesc& result : m_vCmdDesc ) {
		if ( result.getName() == szName )
			return result;
	}
	throw std::out_of_range
		("CmdDesc& CmdDescMap::operator[](const std::string&)");
}

CmdDesc& CmdDescMap::operator[]( unsigned index )
{
	if ( index < m_vCmdDesc.size() )
		return m_vCmdDesc[index];
	throw std::out_of_range("CmdDesc& CmdDescMap::operator[](unsigned)");
}

const CmdDesc& CmdDescMap::getCmdDesc( const std::string& szName ) const
{
	for( const CmdDesc& result : m_vCmdDesc ) {
		if ( result.getName() == szName )
			return result;
	}
	throw std::out_of_range("const CmdDesc& CmdDescMap::getCmdDesc(const "
		"std::string&) const");
}

const CmdDesc& CmdDescMap::getCmdDesc( unsigned index ) const
{
	if ( index < m_vCmdDesc.size() )
		return m_vCmdDesc[index];
	throw std::out_of_range
		("const CmdDesc& CmdDescMap::getCmdDesc(unsigned) const");
}

ErrorCode CmdDescMap::validate( CmdStr& cmdStr ) const {
	ErrorCode result = ErrorCode::INVALID_COMMAND;
	for( unsigned i{0}; i < m_vCmdDesc.size(); i++ ) {
		result = m_vCmdDesc[i].validate( cmdStr );
		if (result != ErrorCode::INVALID_COMMAND) {
			break;
		}
	}
	return result;
}



//
//
// Print help message
//
//

//  
//
std::string indent( unsigned level )
{
	std::string result("   ");
	for( unsigned lvl{0}; lvl < level; lvl++ )
		result += std::string("		");
	return result;
}

//
//
void printConstraints( 
	const std::vector<ConstraintPtr>& vpConstraints, 
	std::ostream& os )
{
	unsigned long long n{vpConstraints.size()};
	if ( n ) {
		os << "(";
		for( unsigned m{0}; m < n; m++ ) {
			os << vpConstraints[m]->toString();
			if ( m < (n-1) )
				os << ",";
		}
		os << ")";
	}
}

//
//
void printCmdDesc( const CmdDesc& cmdDesc, bool bLongVersion, std::ostream& os )
{
	if ( bLongVersion ) {
		unsigned nOptDesc{ cmdDesc.getOptCount() };
		for( unsigned j{0}; j < nOptDesc; j++ ) {
			const OptDesc& optDesc{ cmdDesc.getOptDesc(j) };
			if ( j == 0 ) {
				os << indent(0) 
					<< cmdDesc.getName() 
					<< " - " 
					<< cmdDesc.getWhat() 
					<< "\n";
				os << indent(1) 
					<< "Syntax: ";
				printCmdDesc( cmdDesc, false, os );
			}
			else {
				os << indent(1) << "--" << optDesc.getName();
				std::string s{ optDesc.getShortName() };
				if ( s.length() )
					os << "|-" << s;
				os << ( optDesc.isOptional() 
					? " - Optional. " 
					: " - " )
					<< optDesc.getWhat() 
				   	<< "\n";
			}
			unsigned nParamDesc{ optDesc.getParamCount() };
			for( unsigned k{0}; k < nParamDesc; k++ ) 
			{
				const ParamDescPtr& pParamDesc{ 
					optDesc.getParamDesc(k) };
				os << ( j ? indent(2) : indent(1) )
					<< "<" 
					<< pParamDesc->getName() 
					<< ( pParamDesc->isOptional() 
				   		? "> - Optional. " 
						: "> - " )
				 	<< pParamDesc->getWhat() 
				 	<< " ";
				printConstraints( 
					pParamDesc->getConstraints(), 
					os );
				os << "\n";
			} // k

		} // j
	}
	else {
		unsigned nOptDesc{ cmdDesc.getOptCount() };
		for( unsigned j{0}; j < nOptDesc; j++ ) {
			const OptDesc& optDesc{ cmdDesc.getOptDesc(j) };
			unsigned nParamDesc{ optDesc.getParamCount() };
			if ( j > 0 ) // -> option
			{
				os << ( optDesc.isOptional() 
						? "[" 
						: "" )
					<< "--" 
					<< optDesc.getName();
				const std::string& s{ optDesc.getShortName() };
				if ( s.length() )
					os << "|-" << s;
				 os << ( optDesc.isOptional() && !nParamDesc 
				 		? "]" 
						: " ");
			}
			else // j==0 -> command itself
				os << optDesc.getName() << " ";

			for( unsigned k{0}; k < nParamDesc; k++ ) {
				const ParamDescPtr& pParamDesc{ 
					optDesc.getParamDesc(k) };
				os << ( pParamDesc->isOptional() 
						? "[" 
						: "" ) 
					<< "<" 
					<< pParamDesc->getName();
				printConstraints( pParamDesc->getConstraints(), os );
				os << ">"
					<< ( pParamDesc->isOptional() 
						? "]" 
						: "" );
			} // k
 			
			// close optional-bracket after parameter list
			os << ( optDesc.isOptional() && nParamDesc 
					? "] " 
					: " ");	
		} // j
		os << "\n";
	}
}

//
//
void printHelpMessage( 
	const CmdStr& cs, 
	const CmdDescMap& cdm, 
	std::ostream& os )
{
	// Check for existence of --long / -l
	bool bLongVersion{ (cs.getOptionIndex("long","l") > 0) };
	
	// requested help for a specific command, or all of them?
	bool bPrintAll{ !cs.getCommandParameterCount() }; 
	if ( bPrintAll ) 
	{
		const std::string& szName = cdm.getName();
		for (unsigned i{0};i<szName.length();i++) 
		{
			os << "-";
		}
		os << "\n" 
			<< szName 
			<< "\n";
		for (unsigned i{0};i<szName.length();i++) 
		{
			os << "-";
		}
		os << "\n";
		unsigned nCmdDesc{ cdm.getCmdCount() };
		for( unsigned i{0}; i < nCmdDesc; i++ ) 
		{
			printCmdDesc( cdm.getCmdDesc(i), bLongVersion, os );
		}
	}
	else {
		try {
			printCmdDesc(
				cdm.getCmdDesc( cs.getParameter(0,0) ),
				bLongVersion,
				os );
		}
		catch ( const std::out_of_range& exc ) {
			os << '\''
				<< cs.getCommandParameter(0)
				<< "' is not a command.\n";
		}
	}
}

} // clp
} // elrat
