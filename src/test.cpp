#define BOOST_TEST_MODULE clp test module
#include <boost/test/included/unit_test.hpp>

#include <elrat/clp.h>
using namespace elrat::clp;

#include <stdexcept>
#include <functional>
#include <iostream>
#include <string>
#include <vector>


const CmdDescMap cdm("Available commands", {
	CmdDesc("cmd1","Optional parameters.",
	{
		parameter<Identifier>("param1","mandatory parameter.", Mandatory),
		parameter<Identifier>("param2","optional parameter.")
	}),
	CmdDesc("cmd2","constraints test command",
	{
		parameter<NumN>("A", "Natural number (N)", Mandatory, { 
			constraint<Min,unsigned>(10)}),
		parameter<NumZ>("B", "Whole number (Z)", Mandatory, { 
			constraint<Range,int>(-10,+10) } ),
		parameter<NumR>("C", "Real number (R)", Mandatory, { 
			constraint<Max,double>(10.1) } )
	}),
	CmdDesc("cmd3","Options", {},
	{
		option("flag",'\0',"Set option to do X.", Optional),
		option("option",'o',"Some option.",Optional,{
			parameter<Identifier>("Arg","Blah",Optional)})
	})
});

// shared_ptr as CmdMap template argument 
// - This is more of a does-it-compile-test
BOOST_AUTO_TEST_CASE( CommandMap_with_shared_ptr )
{
	class C { }; 
	typedef std::shared_ptr<C> PC;

	// Does the initialization in the constructor work as intended?
	CmdMap<PC> m({ 
		{"one",PC(new C())}, 
		{"two",PC(new C())} 
	});

	BOOST_CHECK( m.get("one") );							
	BOOST_CHECK( m.get("two") );
	BOOST_CHECK( !m.get("three") );

	m.add("three",PC(new C()));
	BOOST_CHECK( m.get("three")) ;
}

// std::function as CmdMap template argument
// - This is more of a does-it-compile-test
BOOST_AUTO_TEST_CASE( CommandMap_with_stdfunction ) 
{
	CmdMap<std::function<void()>> m;
	
	// Add a non-empty function
	m.add("one",std::function<void()>( [](){} ));

	BOOST_CHECK( m.get("one") );
	BOOST_CHECK( !m.get("two") );
}

// function pointer as CmdMap template argument
BOOST_AUTO_TEST_CASE( CommandMap_with_function_pointer )
{
	typedef void(*FuncPtr)();
	CmdMap<FuncPtr> m;
	FuncPtr p1, p2;
	m.add("one", p1 ); 
	m.add("two", p2 );
	BOOST_CHECK( bool( m.get("one") ) );
	BOOST_CHECK( bool( m.get("two") ) );
	BOOST_CHECK( !m.get("three") );
	BOOST_CHECK( !m.get("help") );
}

BOOST_AUTO_TEST_CASE( CmdStr_parsing_and_accessing_elements )
{
	bool thrown;
	CmdStr c;
	BOOST_CHECK( c.isEmpty() );	
	BOOST_CHECK(!c.getOptionCount());
	BOOST_CHECK_THROW( c.getParameterCount(1), std::out_of_range );
	BOOST_CHECK_THROW( c.getCommandName(), std::out_of_range );						
	BOOST_CHECK_THROW( c.getCommandParameterCount(), std::out_of_range );
	BOOST_CHECK_THROW( c.getCommandParameter(0), std::out_of_range );
	BOOST_CHECK_THROW( c.getParameter(0,0),	std::out_of_range );
	BOOST_CHECK_THROW( c.getParameter(1,0),	std::out_of_range );
	BOOST_CHECK_THROW( c.getParameterAs<int>(1,2), std::out_of_range );
	BOOST_CHECK_THROW( c.getParameterAs<double>(1,2), std::out_of_range );
}

// CmdStr: Options
BOOST_AUTO_TEST_CASE( CmdStr_options )
{
	CmdStr c("cmd1 __arg test12 --option-name \"arg line\" -fD -g=A B C");
	BOOST_CHECK( c.getOptionCount() == 4 );
	BOOST_CHECK( c.getOptionIndex("option-name") == 1 );
	BOOST_CHECK( c.getOptionIndex("f") == 2 );
	BOOST_CHECK( c.getOptionIndex("g") == 3 );
	BOOST_CHECK( c.getOptionIndex("x") == -1 );		// find non-existent option
	BOOST_CHECK( c.getCommandName() == std::string("cmd1") );
	BOOST_CHECK( c.getCommandParameter(0) == std::string("__arg") );
	BOOST_CHECK( c.getCommandParameter(1) == std::string("test12") );
}

// CmdStr: Parameters
BOOST_AUTO_TEST_CASE( CmdStr_parameter )
{
	CmdStr c("x 0x45 0777 -12390 12.21");
	BOOST_CHECK_EQUAL( c.getParameterAs<int>(0,0), 0x45 );
	BOOST_CHECK_EQUAL( c.getParameterAs<int>(0,1), 0777 );
	BOOST_CHECK_EQUAL( c.getParameterAs<int>(0,2), -12390 );
	BOOST_CHECK_EQUAL( c.getParameterAs<float>(0,3), static_cast<float>(12.21) );
}

// Testing regex for decimals
BOOST_AUTO_TEST_CASE( Utility_isDecimal )
{
	BOOST_CHECK( isDecimal("-10") );
	BOOST_CHECK( isDecimal("0") );
	BOOST_CHECK( isDecimal("-0") );
	BOOST_CHECK( isDecimal("+0") );
	BOOST_CHECK( isDecimal("+15") );
	BOOST_CHECK( isDecimal("42") );
	BOOST_CHECK( !isDecimal("") );
	BOOST_CHECK( !isDecimal("0xA") );
	BOOST_CHECK( !isDecimal("42.1") );
	BOOST_CHECK( !isDecimal("067") );

}

// Testing regex for hex
BOOST_AUTO_TEST_CASE( Utility_isHex )
{
	BOOST_CHECK( isHex("0xFF") );		// Check for case insensitivity
	BOOST_CHECK( isHex("0Xff") );		//             "
	BOOST_CHECK( isHex("0XD3adc0d3") );	//             "
	BOOST_CHECK( isHex("0x0"));		// A simple zero

	BOOST_CHECK( !isHex("") );		// Empty string
	BOOST_CHECK( !isHex("0x") );
	BOOST_CHECK( !isHex("0xFFgABCDEFz"));
	BOOST_CHECK( !isHex("0xxFAC"));
	BOOST_CHECK( !isHex("xFAC"));
	BOOST_CHECK( !isHex("+0xFF"));
	BOOST_CHECK( !isHex("-0xFF"));

}

// Testing regex for octal
BOOST_AUTO_TEST_CASE( Utility_isOctal )
{
	BOOST_CHECK( isOctal("01") );
	BOOST_CHECK( isOctal("07") );
	BOOST_CHECK( isOctal("0777") );
	BOOST_CHECK( isOctal("0666") );
	BOOST_CHECK( isOctal("0505") );
	
	BOOST_CHECK( !isOctal("") );		// Empty string
	BOOST_CHECK( !isOctal("505") );		// No preceding '0'
	BOOST_CHECK( !isOctal("0787") );	// Digit '8' is not within 0..7
}

BOOST_AUTO_TEST_CASE( Utility_isFloatingPoint )
{
	BOOST_CHECK( isFloatingPoint("+1") );
	BOOST_CHECK( isFloatingPoint("1") );
	BOOST_CHECK( isFloatingPoint("-1") );
	BOOST_CHECK( isFloatingPoint("1.1") );
	BOOST_CHECK( isFloatingPoint("0777") );
}

// Constraint Interface -> Derived classes: Min, Max, Range
//
// Types 'char' and 'unsigned char' don't work well with the implementation
// probably because of the internal use of stringstreams for conversion.
// Haven't had a closer look yet, because ... low priority.
//
BOOST_AUTO_TEST_CASE( Constraint_templated_converter )
{
	BOOST_CHECK_EQUAL( convert<unsigned short>("0xFF"), static_cast<unsigned short>(255) );
	BOOST_CHECK_EQUAL( convert<double>("12.3456789"), 12.3456789 );
	BOOST_CHECK_EQUAL( convert<int>("0777"), 0777 );
}

//
BOOST_AUTO_TEST_CASE( Constraint_Min ) 
{
	using P = std::shared_ptr<Constraint>;
	P a{ constraint<Min,double>(2.6) };
	BOOST_CHECK( a->check("2.5999") == INVALID_ARGUMENT );
	BOOST_CHECK( a->check("2.6000") == SUCCESS );
	BOOST_CHECK( a->check("+2.6000") == SUCCESS );
	P b{ constraint<Min,int>(0) };
	BOOST_CHECK( b->check("-1") == INVALID_ARGUMENT );
	BOOST_CHECK( b->check("0") == SUCCESS );
	BOOST_CHECK( b->check("1") == SUCCESS );
	BOOST_CHECK( b->check("+1") == SUCCESS );
	BOOST_CHECK( b->check("0xFF") == SUCCESS );

}

//
BOOST_AUTO_TEST_CASE( Constraint_Max ) 
{
	using P = std::shared_ptr<Constraint>;
	P a{ constraint<Max,double>(2.6) };
	BOOST_CHECK( a->check("2.5999") == SUCCESS );
	BOOST_CHECK( a->check("2.6000") == SUCCESS );
	BOOST_CHECK( a->check("+2.6001") == INVALID_ARGUMENT );
	P b{ constraint<Max,int>(0) };
	BOOST_CHECK( b->check("-1") == SUCCESS );
	BOOST_CHECK( b->check("0") == SUCCESS );
	BOOST_CHECK( b->check("1") == INVALID_ARGUMENT );
	BOOST_CHECK( b->check("+1") == INVALID_ARGUMENT );

}

//
BOOST_AUTO_TEST_CASE( Constraint_Rnage ) 
{
	using P = std::shared_ptr<Constraint>;
	P a{ constraint<Range,unsigned>(2,6) };
	BOOST_CHECK( a->check("1") == INVALID_ARGUMENT );
	BOOST_CHECK( a->check("+2") == SUCCESS );
	BOOST_CHECK( a->check("6") == SUCCESS );
	BOOST_CHECK( a->check("7") == INVALID_ARGUMENT );
}

BOOST_AUTO_TEST_CASE( Constraint_Range_with_hex_input )
{
	using P = std::shared_ptr<Constraint>;
	P a{ constraint<Max,unsigned short>(127) };
	BOOST_CHECK( a->check("0x00") == SUCCESS );
	BOOST_CHECK( a->check("0x6A") == SUCCESS );
	BOOST_CHECK( a->check("0x7F") == SUCCESS );				// 127
	BOOST_CHECK( a->check("0x80") == INVALID_ARGUMENT );	 // 128
	BOOST_CHECK( a->check("0xFF") == INVALID_ARGUMENT );
}


BOOST_AUTO_TEST_CASE( ParameterDescriptor_Identifier )
{
	typedef std::shared_ptr<ParamDesc> P;
	P p(new ParamDescIdentifier("identifier",""));
	BOOST_CHECK_EQUAL( p->validate("myidentifier"), SUCCESS );
	BOOST_CHECK_EQUAL( p->validate("MyIdentifier"), SUCCESS );
	BOOST_CHECK_EQUAL( p->validate("m"), SUCCESS );
	BOOST_CHECK_EQUAL( p->validate("_"), SUCCESS );
	BOOST_CHECK_EQUAL( p->validate("_myidentifier"), SUCCESS );
	BOOST_CHECK_EQUAL( p->validate(""), INVALID_ARGUMENT );
	BOOST_CHECK_EQUAL( p->validate("my identifier"), INVALID_ARGUMENT );
	BOOST_CHECK_EQUAL( p->validate("5identifier"), INVALID_ARGUMENT );
	BOOST_CHECK_EQUAL( p->validate("[identifier]"), INVALID_ARGUMENT );
	BOOST_CHECK_EQUAL( p->validate("-identifier"), INVALID_ARGUMENT );
	BOOST_CHECK_EQUAL( p->validate("ident{ifier}"), INVALID_ARGUMENT );

}

BOOST_AUTO_TEST_CASE( ParameterDescriptor_AnyString )
{
	typedef std::shared_ptr<ParamDesc> P;
	P p(new ParamDescString("string", "" ));
	BOOST_CHECK_EQUAL( p->validate("my string can look like anything..."), SUCCESS );
}
	
BOOST_AUTO_TEST_CASE( Command_Descriptor_Map )
{
	CmdStr c;
	
	// cmd1

	c.parseFrom("cmd1 __arg test12");
	BOOST_CHECK_EQUAL( cdm.validate( c ), SUCCESS );
	BOOST_CHECK_EQUAL( c.getCommandName(), std::string("cmd1") );
	
	c.parseFrom("cmd1 valid_identifier 5invalid");
	BOOST_CHECK_EQUAL(c.getCommandParameter(1), std::string("5invalid") );
	BOOST_CHECK_EQUAL( cdm.validate( c ), INVALID_ARGUMENT );

	// cmd2
	
	c.parseFrom("cmd2 10 -10 10.0");
	BOOST_CHECK_EQUAL( c.getOptionCount(), 1 );
	BOOST_CHECK_EQUAL( cdm.validate( c ), SUCCESS );
	BOOST_CHECK_EQUAL( c.getParameterAs<double>(0,2), 10.0 );
	
	c.parseFrom("cmd2 10 -12 10.0");
	BOOST_CHECK_EQUAL( c.getOptionCount(), 1 );
	BOOST_CHECK_EQUAL( cdm.validate( c ), INVALID_ARGUMENT );
	BOOST_CHECK_EQUAL( c.getCommandName(), std::string("cmd2") );

	// cmd3
	
	c.parseFrom("cmd3 --flag -o __ident");
	BOOST_CHECK_EQUAL( cdm.validate( c ), SUCCESS );
	BOOST_CHECK_EQUAL( c.getCommandName(), std::string("cmd3") );
}

BOOST_AUTO_TEST_CASE( BuiltIn_Help ) 
{
	CmdStr c;
	// 1
	c.parseFrom("help");
	BOOST_CHECK_EQUAL( cdm.validate( c ), SUCCESS );
	// 2
	c.parseFrom("help --long");
	BOOST_CHECK_EQUAL( cdm.validate( c ), SUCCESS );
	// 3
	c.parseFrom("help -l");
	BOOST_CHECK_EQUAL( cdm.validate( c ), SUCCESS );
	// 4
	c.parseFrom("help help");
	BOOST_CHECK_EQUAL( cdm.validate( c ), SUCCESS );
	// 5
	c.parseFrom("help help --long");
	BOOST_CHECK_EQUAL( cdm.validate( c ), SUCCESS );
	// 6
	c.parseFrom("help help -l");
	BOOST_CHECK_EQUAL( cdm.validate( c ), SUCCESS );
	// 7
	c.parseFrom("help xxx yyy");
	BOOST_CHECK_EQUAL( cdm.validate( c ), TOO_MANY_ARGUMENTS );
	// 8
	c.parseFrom("help xxx --yyy");
	BOOST_CHECK_EQUAL( cdm.validate( c ), INVALID_OPTION );
	// 9
	c.parseFrom("help xxx --long --xyz");
	BOOST_CHECK_EQUAL( cdm.validate( c ), TOO_MANY_OPTIONS );

}

