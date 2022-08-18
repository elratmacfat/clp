// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/desc.hpp
// Author.......: elratmacfat
// Description..: 
//
#include <elrat/clp/desc.hpp>
#include <regex>

using namespace elrat;
using namespace elrat::clp;

const bool parameter_properties::mandatory{true};
const bool parameter_properties::optional{false};

bool parameter_type::any(std::string_view s)
{
    return (s.size() > 0);
}

bool parameter_type::natural_number(std::string_view s)
{
    return true;
}

bool parameter_type::whole_number(std::string_view s)
{
    return true;
}

bool parameter_type::real_number(std::string_view s)
{
    return true;
}

bool parameter_type::name(std::string_view s)
{
    return true;
}

bool parameter_type::identifier(std::string_view s)
{
    return true;
}


cmd_desc_ptr clp::command(
    const std::string& name,
    const std::string& description,
    const param_desc_vec& parameters,
    const opt_desc_vec& options)
{
    return std::make_shared<cmd_desc>(
        name,description,parameters,options);
}

opt_desc_ptr clp::option(
    const std::string& name,
    const std::string& description,
    const param_desc_vec& parameters )
{
    return std::make_shared<opt_desc>(
        name,description,parameters);
}

param_desc_ptr clp::parameter(
    const std::string& name,
    const std::string& description,
    bool requirement,
    param_type_checker type_checker,
    param_constraint_vec constraints )
{
    return std::make_shared<param_desc>(
        name,description,requirement,type_checker,constraints );
}

desc::desc( const std::string& i, const std::string& d ) 
: _name{i}
, _description{d}
{

}

std::string_view desc::name() const 
{
    return _name;
}

std::string_view desc::description() const
{
    return _description;
}

param_desc::param_desc(
    const std::string& n,
    const std::string& d,
    bool r,
    param_type_checker c,
    param_constraint_vec vc )
: desc(n,d)
, _requirement{r}
, _type_checker{c}
, _constraints{vc}
{

}

bool param_desc::required() const
{
    return _requirement;
}

param_type_checker param_desc::type_checker() const
{
    return _type_checker;
}

const param_constraint_vec& param_desc::constraints() const
{
    return _constraints;
}

opt_desc::opt_desc(
    const std::string& n,
    const std::string& d,
    const param_desc_vec& vp)
: desc(n,d)
, _parameters{vp}
{
}

const param_desc_vec& opt_desc::parameters() const
{
    return _parameters;
}

cmd_desc::cmd_desc(
    const std::string& n,
    const std::string& d,
    const param_desc_vec& vp,
    const opt_desc_vec& vo )
: opt_desc(n,d,vp)
, _options{vo}
{
}

const opt_desc_vec& cmd_desc::options() const
{
    return _options;
}
