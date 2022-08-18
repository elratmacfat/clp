// Project......: Command Line Processor (clp)
// File.........: inc/elrat/clp/desc.hpp
// Author.......: elratmacfat
// Description..: Contains the complete descriptor class hierarchy, including
//                parameter type and constraints.
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

using param_type_checker = std::function<bool(std::string_view)>;

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
    bool any(std::string_view);
    bool natural_number(std::string_view);
    bool whole_number(std::string_view);
    bool real_number(std::string_view);
    bool name(std::string_view);
    bool identifier(std::string_view);
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
protected:
    param_desc_vec          _parameters;
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
private:
    opt_desc_vec            _options;
};

class param_constraint
{
public:
    virtual ~param_constraint() {}
    virtual bool check(std::string_view) const = 0;
    virtual std::string_view string() const = 0;
};

template <class T>
class one_argument
{
public:
    one_argument( const T& t )
    : _t{t}
    {
    }
    virtual ~one_argument() 
    {
    }
protected:
    T _t;
};

template <class T>
class two_arguments
{
public:
    two_arguments( const T& t1, const T& t2 )
    : _t1{t1}
    , _t2{t2}
    {
    }
    virtual ~two_arguments()
    {
    }
protected:
    T _t1;
    T _t2;
};

template <class T>
class many_arguments
{
public:
    template <class TT, class...Args>
    many_arguments( const TT& first, Args...args )
    {
        init( first, args... );
    }
    virtual ~many_arguments()
    {
    }
private:
    template <class TT, class...Args>
    void init( const TT& first, Args...args ) {
        _vt.push_back(first);
        return init( args... );
    }
    template <class TT>
    void init( const TT& last ) {
        _vt.push_back(last);
    }
protected:
    std::vector<T> _vt;
};

template <class T>
class constraint_at_least
: public param_constraint
, public one_argument<T>
{
public:
    using one_argument<T>::one_argument;
    bool check(std::string_view s) const 
    {
        return (util::convert<T>(s) > this->_t);
    }

    std::string_view string() const 
    {
        return "";
    }
};

template <class T>
class constraint_at_most
: public param_constraint
, public one_argument<T>
{
public:
    using one_argument<T>::one_argument;
    bool check(std::string_view s) const 
    {
        return (util::convert<T>(s) < this->_t);
    }

    std::string_view string() const 
    {
        return "";
    }
};

template <class T>
class constraint_is_not
: public param_constraint
, public one_argument<T>
{
public:
    using one_argument<T>::one_argument;
    bool check(std::string_view s) const 
    {
        return (util::convert<T>(s) != this->_t);
    }

    std::string_view string() const 
    {
        return "";
    }
};


template <class T>
class constraint_in_range
: public param_constraint
, public two_arguments<T>
{
public:
    using two_arguments<T>::two_arguments;
    bool check(std::string_view s) const 
    {
        T x = util::convert<T>(s);
        return ( x >= this->_t1 && x <= this->_t2 );
    }

    std::string_view string() const 
    {
        return "";
    }
};

template <class T>
class constraint_in
: public param_constraint
, public many_arguments<T>
{
public:
    using many_arguments<T>::many_arguments;
    bool check(std::string_view s) const 
    {
        T x = util::convert<T>(s);
        for( auto& t : this->_vt ) 
            if ( x == t ) 
                return true;
        return false;
    }

    std::string_view string() const 
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
param_constraint_ptr constraint::in(TT...t)
{
    return std::make_shared<constraint_in<T>>(t...);
}


} // namespace clp
} // namespace elrat

#endif // include guard
