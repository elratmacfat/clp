// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/desc.hpp
// Author.......: elratmacfat
// Description..: 
//
#include <elrat/clp/descriptors.hpp>
#include <elrat/clp/regex.hpp>

using namespace elrat;
using namespace elrat::clp;

const bool mandatory{true};
const bool optional{false};

bool clp::any(const std::string& s)
{
    return (s.size() > 0);
}

bool clp::natural_number(const std::string& s)
{
    return regex::is_positive_decimal(s) ||
        regex::is_hexadecimal(s);
}

bool clp::whole_number(const std::string& s)
{   
    return regex::is_decimal(s) 
        || regex::is_hexadecimal(s);
}

bool clp::real_number(const std::string& s)
{
    return regex::is_floating_point(s);
}

bool clp::name(const std::string& s)
{
    return regex::is_name(s);
}

bool clp::identifier(const std::string& s)
{
    return regex::is_identifier(s);
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

//-----------------------------------------------------------------------------

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

bool operator<(const desc& lhs, const desc& rhs) 
{
    return ( lhs.name().compare( rhs.name() ) < 0 );
}

//-----------------------------------------------------------------------------

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

vcode param_desc::validate(const std::string& p) 
{
    if (!_type_checker(p))
        return vcode::param_invalid;
    for(auto& c : _constraints)
        if (!c->validate(p))
            return vcode::param_invalid;
    return vcode::match;
}

//-----------------------------------------------------------------------------

opt_desc::opt_desc(
    const std::string& n,
    const std::string& d,
    const param_desc_vec& vp)
: desc(n,d)
, _parameters{vp}
{
    // Ensure that an optional parameter is not followed by one that is 
    // required. Also count the number of required parameters, for future
    // validation processes.
    this->_n_required = 0;
    bool prev_optional{false};
    for( auto& p : vp )
    {
        if (p->required())
        {
            _n_required++;
            if ( prev_optional )
                throw std::invalid_argument(
                    "Optional parameters cannot be followed by one that "
                    "is required. Revisit your command descriptor "
                    "initialization.");
        }
        else
        {
            prev_optional = true;
        }
    }
}

const param_desc_vec& opt_desc::parameters() const
{
    return _parameters;
}

vcode opt_desc::validate( const std::vector<std::string>& params )
{
    if ( !params.size() || params[0] != this->name() )
        return vcode::opt_invalid;

    // Check if parameters are missing or too many have been provided.
    // Note that the first element in 'params' is the option name, hence
    // the fiddling with +-1.
    auto param_count{ params.size() - 1 }; 
    if ( param_count < _n_required )
        return vcode::param_missing;
    if ( param_count > _parameters.size() )
        return vcode::param_invalid;

    // Let each parameter check for themselves.
    for( int i{1}; i <= params.size(); i++ )
    {
        vcode result {_parameters[i-1]->validate(params[i])};
        if ( result != vcode::match )
            return result;
    }
}

//-----------------------------------------------------------------------------

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

vcode cmd_desc::validate( const parser::data& data )
{
    if (!data)
        return vcode::cmd_invalid;
    
    // Check command and its parameters
    // (opt_desc refers to the base class here)
    vcode v{ opt_desc::validate( data.cmd_param_vec() ) };
    if ( v != vcode::match ) 
    {
        if ( v == vcode::opt_invalid ) 
            v = vcode::cmd_invalid;
        return v;
    }

    // Each option descriptor gets to see every option passed.
    //
    vcode result{};
    for( int i{0}; i < data.opt_count(); i++ )
    {
        auto v{ data.opt_param_vec(i+1) };
        for( auto odesc : _options )
        {
            result = odesc->validate(v);
            
            // Found a match, abort inner loop, and go on with
            // the next iteration
            if ( result == vcode::match )
                break;

            // Option name matched, but its parameters didnt fit.
            // Thus, the validation as a whole has failed.
            if ( result != vcode::opt_invalid )
                return result;
                
        }
    }
    return result;
}


//-----------------------------------------------------------------------------
