// Project......: Command Line Processor (clp)
// File.........: src/text.cpp
// Author.......: elratmacfat
// Description..: 
//
#include <elrat/clp/text.hpp>


std::ostream& operator<<(std::ostream& os, const elrat::clp::data& data )
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

std::ostream& operator<<(std::ostream& os,const elrat::clp::parser::error& err)
{
    if ( !err ) {
        return os;
    }
    os     << "Error: \"" << err.message() << "\"\n";
    if ( err.source().size() ) {
        os << "=> Source: \"" << err.source() << "\"\n";
    }
    if ( err.expression().size() ) {
        os << "=> Expression: \"" << err.expression() << "\"\n";
    }
    return os;
}


