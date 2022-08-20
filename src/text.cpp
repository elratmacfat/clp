// Project......: Command Line Processor (clp)
// File.........: src/text.cpp
// Author.......: elratmacfat
// Description..: 
//
#include <elrat/clp/text.hpp>

std::ostream& operator<<(
    std::ostream& os, 
    const elrat::clp::parser::data& data)
{
    if (data.empty()) 
        return os;
    
    os << "Command............: \""
        << data.cmd() 
        << "\"\n";
    for( int i{1}; i < 1+data.cmd_param_count(); i++ )
    {
        os << "-> Parameter #" 
            << i 
            << "....: \"" 
            << data.cmd_param(i) 
            << "\"\n";
    }
    for( int i{1}; i < 1+data.opt_count(); i++ )
    {        
        os << "-> Option #" 
            << i
            << ".......: \""
            << data.opt(i) 
            << "\"\n";
        for( int k{1}; k < 1+data.opt_param_count(i); k++ ) 
        {
            os << "  -> Parameter #" 
                << k 
                << "..: \"" 
                << data.opt_param(i,k) 
                << "\"\n";
        }
    }
    return os;
}

std::ostream& operator<<(
    std::ostream& os,
    const elrat::clp::parser::error& err)
{
    if ( !err ) {
        return os;
    }
    os << err();
    if ( err.message().size() )
        os << ": [" << err.message() << "]";
    if ( err.source().size() )
        os << " in \"" << err.source() << "\"";
    os << '\n';
    return os;
}

std::ostream& operator<<(
    std::ostream& os,
    const elrat::clp::parser::error::code& c)
{
    using code = elrat::clp::parser::error::code;
    switch (c) 
    {
    case code::success: os << "success"; break;
    case code::failure: os << "undefined failure"; break;
    case code::syntax: os << "syntax error"; break;
    case code::redundant: os << "redundant option"; break;
    default:
        os << "unknown error";
    }
    return os;
}
