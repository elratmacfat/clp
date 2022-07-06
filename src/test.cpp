

#include <elrat/clp.h>
#include <elrat/testbench.h>

#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace elrat::clp;
using namespace elrat::testbench;
using namespace std;

// In order of dependence
void TestCmdMap();
void TestCmdStr();          // 'CmdStr': independent
void TestUtility();         // 
void TestConstraint();      // 'Constraint' interface, 'Min', 'Max', 'Range': depe
void TestParamDesc();       // 'ParamDesc/ParameterDescriptor': depends on 'Constraint'
void TestOptDesc();         // 'OptDesc/OptionDescriptor': depends on 'ParamDesc'


// Dependent examinees
void TestCmdDescMap();
void TestBuiltInHelp();

const CmdDescMap cdm("Available commands", {
    CmdDesc("cmd1","Optional parameters.",
    {
        parameter<Identifier>("param1","mandatory parameter.", Mandatory),
        parameter<Identifier>("param2","optional parameter.")
    }),
    CmdDesc("cmd2","constraints test command",
    {
        parameter<NumN>("A", "Natural number (N)", Mandatory, { constraint<Min,unsigned>(10) } ),
        parameter<NumZ>("B", "Whole number (Z)", Mandatory, { constraint<Range,int>(-10,+10) } ),
        parameter<NumR>("C", "Real number (R)", Mandatory, { constraint<Max,double>(10.1) } )
    }),
    CmdDesc("cmd3","Options", {},
    {
        option("flag",'\0',"Set option to do X.", Optional),
        option("option",'o',"Some option.",Optional,{parameter<Identifier>("Arg","Blah",Optional)})
    })
});



int main ( int argc, char* argv[] )
{
    elrat::clp::printHelpMessage( CmdStr("help --long"), cdm, cout );

    TestCmdMap();

    TestCmdStr();

    TestUtility();

    TestConstraint();

    TestParamDesc();


    TestCmdDescMap();
    TestBuiltInHelp();


    return 0;
}

//
//  TestCase Cmd Map
//
//	This is more of a does-it-even-compile-test
//
//	Testing if several callable constructs can be bound
//	to the CmdMap's template parameter.
//
void TestCmdMap()
{
    {   // shared_ptr
        TestCase t("CmdMap with std::shared_ptr");
        class C { };
        typedef std::shared_ptr<C> PC;
        CmdMap<PC> m({
            {"one",PC(new C())},
            {"two",PC(new C())}
        });
        t.check( bool(m.get("one")) );
        t.check( bool(m.get("two")) );
        t.check( !m.get("three") );

        m.add("three",PC(new C()));
        t.check( bool(m.get("three")) );
    }
    { // std::function
        TestCase t("CmdMap with std::function");
        CmdMap<std::function<void()>> m;
        m.add("one",std::function<void()>( [](){} ));    // empty lambda expression.
        t.check( bool( m.get("one") ) );
        t.check( !m.get("two") );
    }
    { // function pointer
        typedef void(*FuncPtr)();
        TestCase t("CmdMap with function pointer");
        CmdMap<FuncPtr> m;
        FuncPtr ptr{0};
        m.add("one", ptr + 1 ); // to make sure the ptr is not 0 and evaluates to "true".
        m.add("two", ptr + 2 );
        t.check( bool( m.get("one") ) );
        t.check( bool( m.get("two") ) );
        t.check( !m.get("three") );
        t.check( !m.get("help") );
    }



}

//
//  CmdStr / CommandStrings
//
//  Checks if issued command strings are parsed correctly.
//  In addition, the behaviour upon accessing existing and
//  non-existing elements is tested.
//
void TestCmdStr() {
    { // CmdStr: Empty
        TestCase t("CmdStr: Empty");
        bool thrown;
        CmdStr c;
        t.check( c.isEmpty() );         // Ask for container size -> OK
        t.check(!c.getOptionCount());   //          "

        t.checkThrow( std::bind( &CmdStr::getParameterCount, &c, 1 ) );     // Get the parameter count of a non-existent option.

        t.checkThrow( std::bind( &CmdStr::getCommandName, &c ) );           // Access non-existent element -> Throw
        t.checkThrow( std::bind( &CmdStr::getCommandParameterCount, &c ) ); //                  "
        t.checkThrow( std::bind( &CmdStr::getCommandParameter, &c, 0 ) );   //                  "
        t.checkThrow( std::bind( &CmdStr::getParameter, &c, 0, 0 ) );       // Same as getCommandParameter(0)
        t.checkThrow( std::bind( &CmdStr::getParameter, &c, 1, 0 ) );       // Access non-existent element -> Throw

        thrown = false;
        try { c.getParameterAs<int>(1,2); } catch ( ... ) { thrown = true; }
        t.check( thrown );

        thrown = false;
        try { c.getParameterAs<double>(1,2); } catch ( ... ) { thrown = true; }
        t.check( thrown );

    }
    { // CmdStr: Options
        TestCase t("CmdStr: Options");
        CmdStr c("cmd1 __arg test12 --option-name \"arg line\" -fD -g=A B C");
        t.check( c.getOptionCount() == 4 );
        t.check( c.getOptionIndex("option-name") == 1 );
        t.check( c.getOptionIndex("f") == 2 );
        t.check( c.getOptionIndex("g") == 3 );
        t.check( c.getOptionIndex("x") == -1 );     // find non-existent option
        t.check( c.getCommandName() == std::string("cmd1") );
        t.check( c.getCommandParameter(0) == std::string("__arg") );
        t.check( c.getCommandParameter(1) == std::string("test12") );
    }
    { // CmdStr: getParameterAs<>
        TestCase t("CmdStr: getParameterAs<>");
        CmdStr c("x 0x45 0777 -12390 12.21");
        t.checkEqual( c.getParameterAs<int>(0,0), 0x45 );
        t.checkEqual( c.getParameterAs<int>(0,1), 0777 );
        t.checkEqual( c.getParameterAs<int>(0,2), -12390 );
        t.checkEqual( c.getParameterAs<float>(0,3), static_cast<float>(12.21) );
    }
}

void TestUtility()
{
    { // Testing regex for strings representing decimal numbers.
        TestCase t("isDecimal()");
        t.check( isDecimal("-10") );
        t.check( isDecimal("0") );
        t.check( isDecimal("-0") );
        t.check( isDecimal("+0") );
        t.check( isDecimal("+15") );
        t.check( isDecimal("42") );
        t.check( !isDecimal("") );
        t.check( !isDecimal("0xA") );
        t.check( !isDecimal("42.1") );
        t.check( !isDecimal("067") );
    }
    { // Testing regex for hex 
        TestCase t("isHex()");
        t.check( isHex("0xFF") );
        t.check( isHex("0Xff") );
        t.check( isHex("0XD3adc0d3") );
        t.check( isHex("0x0"));
        t.check( !isHex("") );
        t.check( !isHex("0x") );
        t.check( !isHex("0xFFgABCDEFz"));
        t.check( !isHex("0xxFAC"));
        t.check( !isHex("xFAC"));
        t.check( !isHex("+0xFF"));
        t.check( !isHex("-0xFF"));
    }
    { // Testing regex for octal 
        TestCase t("isOctal()");
        t.check( isOctal("01") );
        t.check( isOctal("07") );
        t.check( isOctal("0777") );
        t.check( isOctal("0666") );
        t.check( isOctal("0505") );
        t.check( !isOctal("") );
        t.check( !isOctal("505") );
        t.check( !isOctal("0787") );
    }
    { // Testing regex for floating point numbers
        TestCase t("isFloatingPoint()");
        t.check( isFloatingPoint("+1") );
        t.check( isFloatingPoint("1") );
        t.check( isFloatingPoint("-1") );
        t.check( isFloatingPoint("1.1") );
        t.check( isFloatingPoint("0777") );
    }
}

//
//  Constraint Interface -> Derived classes: Min, Max, Range
//
//  Types 'char' and 'unsigned char' don't work well with the implementation
//  probably because of the internal use of stringstreams for conversion.
//  Haven't had a closer look yet, because ... low priority.
//
void TestConstraint() {
    typedef std::shared_ptr<Constraint> P;
    {
        TestCase t("Constraint interface: templated converter function");

        //t.checkEqual( convert<unsigned char>("0xFF"), static_cast<unsigned char>(255) );

        t.checkEqual( convert<unsigned short>("0xFF"), static_cast<unsigned short>(255) );

        t.checkEqual( convert<double>("12.3456789"), 12.3456789 );

        t.checkEqual( convert<int>("0777"), 0777 );
    }
    {
        TestCase t("Constraint interface: Min");
        P a{ constraint<Min,double>(2.6) };
        t.check( a->check("2.5999") == INVALID_ARGUMENT );
        t.check( a->check("2.6000") == SUCCESS );
        t.check( a->check("+2.6000") == SUCCESS );
        P b{ constraint<Min,int>(0) };
        t.check( b->check("-1") == INVALID_ARGUMENT );
        t.check( b->check("0") == SUCCESS );
        t.check( b->check("1") == SUCCESS );
        t.check( b->check("+1") == SUCCESS );
        t.check( b->check("0xFF") == SUCCESS );
    }
    {
        TestCase t("Constraint interface: Max");
        P a{ constraint<Max,double>(2.6) };
        t.check( a->check("2.5999") == SUCCESS );
        t.check( a->check("2.6000") == SUCCESS );
        t.check( a->check("+2.6001") == INVALID_ARGUMENT );
        P b{ constraint<Max,int>(0) };
        t.check( b->check("-1") == SUCCESS );
        t.check( b->check("0") == SUCCESS );
        t.check( b->check("1") == INVALID_ARGUMENT );
        t.check( b->check("+1") == INVALID_ARGUMENT );
    }
    {
        TestCase t("Constraint interface: Range");
        P a{ constraint<Range,unsigned>(2,6) };
        t.check( a->check("1") == INVALID_ARGUMENT );
        t.check( a->check("+2") == SUCCESS );
        t.check( a->check("6") == SUCCESS );
        t.check( a->check("7") == INVALID_ARGUMENT );
    }
    {
        TestCase t("Constraint interface: [Min,Max,Range] with hex input");
        P a{ constraint<Max,unsigned short>(127) };
        t.check( a->check("0x00") == SUCCESS );
        t.check( a->check("0x6A") == SUCCESS );
        t.check( a->check("0x7F") == SUCCESS );             // 127
        t.check( a->check("0x80") == INVALID_ARGUMENT );     // 128
        t.check( a->check("0xFF") == INVALID_ARGUMENT );
    }
}


void TestParamDesc() {
    typedef std::shared_ptr<ParamDesc> P;
    { // ParamDesc -> Identifier
        TestCase t("ParamDesc -> Identifier");
        P p(new ParamDescIdentifier("identifier",""));
        t.checkEqual( p->validate("myidentifier"), SUCCESS );
        t.checkEqual( p->validate("MyIdentifier"), SUCCESS );
        t.checkEqual( p->validate("m"), SUCCESS );
        t.checkEqual( p->validate("_"), SUCCESS );
        t.checkEqual( p->validate("_myidentifier"), SUCCESS );
        t.checkEqual( p->validate(""), INVALID_ARGUMENT );
        t.checkEqual( p->validate("my identifier"), INVALID_ARGUMENT );
        t.checkEqual( p->validate("5identifier"), INVALID_ARGUMENT );
        t.checkEqual( p->validate("[identifier]"), INVALID_ARGUMENT );
        t.checkEqual( p->validate("-identifier"), INVALID_ARGUMENT );
        t.checkEqual( p->validate("ident{ifier}"), INVALID_ARGUMENT );
    }
    { // ParamDesc -> String
        TestCase t("ParamDesc -> String");
        P p(new ParamDescString("string", "" ));
        t.checkEqual( p->validate("my string can look like anything..."), SUCCESS );
    }
}






void TestCmdDescMap() {


    CmdStr c;

    TestCase t("CmdDescMap.validate()");

    // cmd1
    c.parseFrom("cmd1 __arg test12");

    t.checkEqual( cdm.validate( c ), SUCCESS );

    t.checkEqual( c.getCommandName(), std::string("cmd1") );


    c.parseFrom("cmd1 valid_identifier 5invalid");
    t.checkEqual(c.getCommandParameter(1), std::string("5invalid") );
    t.checkEqual( cdm.validate( c ), INVALID_ARGUMENT );

    // cmd2
    c.parseFrom("cmd2 10 -10 10.0");
    t.checkEqual( c.getOptionCount(), 1 );
    t.checkEqual( cdm.validate( c ), SUCCESS );
    t.checkEqual( c.getParameterAs<double>(0,2), 10.0 );
    c.parseFrom("cmd2 10 -12 10.0");
    t.checkEqual( c.getOptionCount(), 1 );
    t.checkEqual( cdm.validate( c ), INVALID_ARGUMENT );
    t.checkEqual( c.getCommandName(), std::string("cmd2") );

    // cmd3
    c.parseFrom("cmd3 --flag -o __ident");
    t.checkEqual( cdm.validate( c ), SUCCESS );
    t.checkEqual( c.getCommandName(), std::string("cmd3") );

    c.parseFrom("cmd3");

}

void TestBuiltInHelp()
{
    TestCase t("CmdDescMap, Built-in 'help'");
    CmdStr c;
    // 1
    c.parseFrom("help");
    t.checkEqual( cdm.validate( c ), SUCCESS );
    // 2
    c.parseFrom("help --long");
    t.checkEqual( cdm.validate( c ), SUCCESS );
    // 3
    c.parseFrom("help -l");
    t.checkEqual( cdm.validate( c ), SUCCESS );
    // 4
    c.parseFrom("help help");
    t.checkEqual( cdm.validate( c ), SUCCESS );
    // 5
    c.parseFrom("help help --long");
    t.checkEqual( cdm.validate( c ), SUCCESS );
    // 6
    c.parseFrom("help help -l");
    t.checkEqual( cdm.validate( c ), SUCCESS );
    // 7
    c.parseFrom("help xxx yyy");
    t.checkEqual( cdm.validate( c ), TOO_MANY_ARGUMENTS );
    // 8
    c.parseFrom("help xxx --yyy");
    t.checkEqual( cdm.validate( c ), INVALID_OPTION );
    // 9
    c.parseFrom("help xxx --long --xyz");
    t.checkEqual( cdm.validate( c ), TOO_MANY_OPTIONS );

}


