// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/desc.hpp
// Author.......: elratmacfat
// Description..: Contains the complete descriptor class hierarchy, including
//                parameter type and constraints.
//                
//
//
//
//
#ifndef ELRAT_CLP_DESC_HPP
#define ELRAT_CLP_DESC_HPP

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <elrat/clp/parser.hpp>
#include <elrat/clp/util.hpp>

namespace elrat {
namespace clp {

//-----------------------------------------------------------------------------
// Forward declaration
//-----------------------------------------------------------------------------

class cmd_desc;
using cmd_desc_ptr = std::shared_ptr<cmd_desc>;

class opt_desc;
using opt_desc_ptr = std::shared_ptr<opt_desc>;
using opt_desc_vec = std::vector<opt_desc_ptr>;

class param_desc;
using param_desc_ptr = std::shared_ptr<param_desc>;
using param_desc_vec = std::vector<param_desc_ptr>;

using param_type_checker = std::function<bool(const std::string&)>;

class param_constraint;
using param_constraint_ptr = std::shared_ptr<param_constraint>;
using param_constraint_vec = std::vector<param_constraint_ptr>;

//-----------------------------------------------------------------------------
// Factories, aliases and wrapper with the intent of providing a neat interface
// to the library user (who shall not have to deal with dozens of abbreviations
// and internals.
//
//
//-----------------------------------------------------------------------------

namespace parameter_properties 
{
    extern const bool mandatory;
    extern const bool optional;
}

namespace parameter_type 
{
    bool any(const std::string&);
    bool natural_number(const std::string&);
    bool whole_number(const std::string&);
    bool real_number(const std::string&);
    bool name(const std::string&);
    bool identifier(const std::string&);
}

namespace constraint
{
    template <class T> 
    param_constraint_ptr at_least(const T&);
    
    template <class T> 
    param_constraint_ptr at_most(const T&);
    
    template <class T> 
    param_constraint_ptr in_range(const T&, const T&);
    
    template <class T> 
    param_constraint_ptr is_not(const T&);
    
    template <class...T> 
    param_constraint_ptr in(T...);
}

enum class vcode 
{
     match          // OK
    ,cmd_invalid    // command name does not match 
    ,param_invalid  // Parameter type or value mismatch, or too many parameters
    ,param_missing  // Required parameter is missing
    ,opt_invalid    // Option provided that does not exist.
};

cmd_desc_ptr command(
    const std::string& name,
    const std::string& description = "",
    const param_desc_vec& parameters = {},
    const opt_desc_vec& options = {}
);

opt_desc_ptr option(
    const std::string& name,
    const std::string& description = "",
    const param_desc_vec& parameters = {}
);

param_desc_ptr parameter(
    const std::string& name,
    const std::string& description = "",
    bool requirement = parameter_properties::mandatory,
    param_type_checker type_checker = parameter_type::any,
    param_constraint_vec constraints = {}
);

class desc 
{
public:
    desc() = delete;
    desc(const std::string&, const std::string&);
    desc(const desc&) = default;
    desc(desc&&) = default;
    ~desc() = default;
    desc& operator=(const desc&) = default;
    desc& operator=(desc&&) = default;
    std::string_view name() const;
    std::string_view description() const;
private:
    std::string _name;
    std::string _description;
};

class param_desc 
: public desc
{
public:
    param_desc(
        const std::string&,
        const std::string&,
        bool,
        param_type_checker,
        param_constraint_vec );
    bool required() const;
    param_type_checker type_checker() const;
    const param_constraint_vec& constraints() const;
    vcode validate(const std::string&);
private:
    bool                    _requirement;
    param_type_checker      _type_checker;
    param_constraint_vec    _constraints;
};

class opt_desc 
: public desc
{
public:
    opt_desc(
        const std::string&,
        const std::string&,
        const param_desc_vec& );
    const param_desc_vec& parameters() const;
    vcode validate( const std::vector<std::string>& );
protected:
    param_desc_vec  _parameters;
    int             _n_required;
};

class cmd_desc
: public opt_desc
{
public:
    cmd_desc(
        const std::string&,
        const std::string&,
        const param_desc_vec&,
        const opt_desc_vec& );
    const opt_desc_vec& options() const;
    
    vcode validate( const parser::data& );
private:
    opt_desc_vec            _options;
};

// 
class param_constraint
{
public:
    virtual ~param_constraint() {}
    virtual bool validate(const std::string&) const = 0;
    virtual const std::string& to_string() const = 0;
};

// Base class for concrete parameter constraints
//
// Provides a constructor that takes an arbitrary amount of arguments of the
// same type. Subclasses can access the protected elements with:
//    this->values.at(i)
//
template <class T>
class args_t
{
public:
    template <class TT, class...Args>
    args_t( const TT& first, Args...args )
    {
        init( first, args... );
    }
    virtual ~args_t()
    {
    }
private:
    template <class TT, class...Args>
    void init( const TT& first, Args...args ) {
        values.push_back(first);
        return init( args... );
    }
    template <class TT>
    void init( const TT& last ) {
        values.push_back(last);
    }
protected:
    std::vector<T> values;
};

template <class T>
class constraint_at_least
: public param_constraint
, public args_t<T>
{
public:
    using args_t<T>::args_t;
    bool validate(const std::string& s) const 
    {
        return (util::convert<T>(s) >= this->values.at(0));
    }

    const std::string& to_string() const 
    {
        return "";
    }
};

template <class T>
class constraint_at_most
: public param_constraint
, public args_t<T>
{
public:
    using args_t<T>::args_t;
    bool validate(const std::string& s) const 
    {
        return (util::convert<T>(s) <= this->values.at(0));
    }

    const std::string& to_string() const 
    {
        return "";
    }
};

template <class T>
class constraint_is_not
: public param_constraint
, public args_t<T>
{
public:
    using args_t<T>::args_t;
    bool validate(const std::string& s) const 
    {
        return (util::convert<T>(s) != this->values.at(0));
    }

    const std::string& to_string() const 
    {
        return "";
    }
};


template <class T>
class constraint_in_range
: public param_constraint
, public args_t<T>
{
public:
    using args_t<T>::args_t;
    bool validate(const std::string& s) const 
    {
        T x = util::convert<T>(s);
        const T& t1 = this->values.at(0);
        const T& t2 = this->values.at(1);
        return ( x >= t1 && x <= t2 );
    }

    const std::string& to_string() const 
    {
        return "";
    }
};

template <class T>
class constraint_in
: public param_constraint
, public args_t<T>
{
public:
    using args_t<T>::args_t;
    bool validate(const std::string& s) const 
    {
        T x = util::convert<T>(s);
        for( auto& t : this->values ) 
            if ( x == t ) 
                return true;
        return false;
    }

    const std::string& to_string() const 
    {
    return "";
    }
};

template <class T> 
param_constraint_ptr constraint::at_least(const T& t) 
{
    return std::make_shared<constraint_at_least<T>>(t);
}

template <class T> 
param_constraint_ptr constraint::at_most(T&& t)
{
    return std::make_shared<constraint_at_most<T>>(std::move(t));
}

template <class T> 
param_constraint_ptr constraint::in_range(T&& t1, T&& t2)
{
    return std::make_shared<constraint_in_range<T>>(t1,t2);
}

template <class T> 
param_constraint_ptr constraint::is_not(T&& t)
{
    return std::make_shared<constraint_is_not<T>>(std::move(t));
}

template <class T, class...TT> 
param_constraint_ptr constraint::in(T first, TT...rest)
{
    return std::make_shared<constraint_in<T>>(first, rest...);
}


} // namespace clp
} // namespace elrat

#endif // include guard
