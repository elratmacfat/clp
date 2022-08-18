// Project......: Command Line Processor (clp)
// File.........: src/data.cpp
// Author.......: elratmacfat
// Description..: 

#include <elrat/clp/data.hpp>

#include <stdexcept>

using namespace elrat;

clp::data::data( clp::data::structure&& s )
: _s{ std::move(s) }
{
}

bool clp::data::empty() const
{
    return ( _s.size() == 0 );
}

clp::data::operator bool() const 
{
    return (_s.size() > 0 );
}

int clp::data::cmd_param_count() const
{
    return _s.at(0).size() - 1;
}


int clp::data::opt_count() const
{
    if (this->empty())
        throw std::out_of_range("Out Of Range: clp::data::opt_count()");
    return _s.size() - 1;
}

int clp::data::opt_exists(const std::string& s) const
{
    for( int i{1}; i < _s.size(); i++ )
    {
        if( _s.at(i).at(0) == s )
            return i;
    }
    return 0;
}

int clp::data::opt_param_count(int i) const
{
    return _s.at(i).size() - 1;
}
    
std::string_view clp::data::cmd() const
{
    return _s.at(0).at(0);
}
    
std::string_view clp::data::cmd_param(int i) const
{   
    return _s.at(0).at(i);
}
   
std::string_view clp::data::opt(int i) const 
{
    return _s.at(i).at(0);
}

std::string_view clp::data::opt_param(int i,int k) const
{
    return _s.at(i).at(k);
}


