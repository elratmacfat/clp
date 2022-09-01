// Project......: Command Line Processor (clp)
// File.........: /elrat/clp/descriptors.hpp
// Author.......: elratmacfat
// Description..: 
//
#ifndef ELRAT_CLP_DESCRIPTORS_HPP
#define ELRAT_CLP_DESCRIPTORS_HPP

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <elrat/clp/parser.hpp>

namespace elrat {
namespace clp {

class CommandDescriptors;

class CommandDescriptor;
using CommandDescriptorPtr = std::shared_ptr<CommandDescriptor>;

class OptionDescriptor;
using OptionDescriptorPtr = std::shared_ptr<OptionDescriptor>;
using OptionDescriptors = std::vector<OptionDescriptorPtr>;

class ParameterDescriptor;
using ParameterDescriptorPtr = std::shared_ptr<ParameterDescriptor>;
using ParameterDescriptors = std::vector<ParameterDescriptorPtr>;

using TypeChecker = std::function<bool(const std::string&)>;

class Constraint;
using ConstraintPtr = std::shared_ptr<Constraint>;
using Constraints = std::vector<ConstraintPtr>;

extern const bool Mandatory;
extern const bool Optional;

bool Any(const std::string&);
bool NaturalNumber(const std::string&);
bool WholeNumber(const std::string&);
bool RealNumber(const std::string&);
bool Name(const std::string&);
bool Identifier(const std::string&);
bool Path(const std::string&);

enum class ValidationResult 
{
     Valid
    ,InvalidCommand
    ,InvalidParameter 
    ,MissingParameter
    ,InvalidOption 
};

CommandDescriptorPtr Command(
    const std::string& name,
    const std::string& description = "",
    const ParameterDescriptors& parameters = {},
    const OptionDescriptors& options = {}
);

OptionDescriptorPtr Option(
    const std::string& name,
    const std::string& description = "",
    const ParameterDescriptors& parameters = {}
);

ParameterDescriptorPtr Parameter(
    const std::string& name,
    const std::string& description = "",
    bool requirement = Mandatory,
    TypeChecker type_checker = Any,
    Constraints constraints = {}
);

template <class T> ConstraintPtr AtLeast(const T& t);
template <class T> ConstraintPtr AtMost(T&& t);
template <class T> ConstraintPtr InRange(T&& t1, T&& t2);
template <class T> ConstraintPtr IsNot(T&& t);
template <class T, class...TT> ConstraintPtr In(T first, TT...others);


//-----------------------------------------------------------------------------

class HasName
{
public:
    HasName(const std::string&);
    const std::string& getName() const;
private:
    std::string name;
};

class HasDescription
{
public:
    HasDescription(const std::string&);
    const std::string& getDescription() const;
private:
    std::string description;
};

//-----------------------------------------------------------------------------

class ParameterDescriptor 
: public HasName
, public HasDescription
{
public:
    ParameterDescriptor(
        const std::string&,
        const std::string&,
        bool,
        TypeChecker,
        Constraints );
    bool parameterIsRequired() const;
    TypeChecker getTypeChecker() const;
    const Constraints& getConstraints() const;
    ValidationResult validate(const std::string&);
private:
    bool        required;
    TypeChecker type_checker;
    Constraints constraints;
};

//-----------------------------------------------------------------------------

class OptionDescriptor 
: public HasName
, public HasDescription
{
public:
    OptionDescriptor(
        const std::string&,
        const std::string&,
        const ParameterDescriptors& );
    const ParameterDescriptors& getParameters() const;
    ValidationResult validate( const std::vector<std::string>& );
protected:
    ParameterDescriptors    parameters;
    int                     numberOfRequiredParameters;
};

//-----------------------------------------------------------------------------

class CommandDescriptor
: public OptionDescriptor
{
public:
    CommandDescriptor(
        const std::string&,
        const std::string&,
        const ParameterDescriptors&,
        const OptionDescriptors& );
    const OptionDescriptors& getOptions() const;
    
    ValidationResult validate( const CommandLine& );
private:
    OptionDescriptors options;
};

//-----------------------------------------------------------------------------

class CommandDescriptors
{
};

//-----------------------------------------------------------------------------

class Constraint
{
public:
    virtual ~Constraint() {}
    virtual bool validate(const std::string&) const = 0;
    virtual const std::string& to_string() const = 0;
};

template <class T>
T convert(const std::string& arg) 
{
    std::stringstream ss;
    ss << arg;
    T result;
    ss >> result;
    return result;
}

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
: public Constraint
, public args_t<T>
{
public:
    using args_t<T>::args_t;
    bool validate(const std::string& s) const 
    {
        return (convert<T>(s) >= this->values.at(0));
    }

    const std::string& to_string() const 
    {
        return "";
    }
};

template <class T>
class constraint_at_most
: public Constraint
, public args_t<T>
{
public:
    using args_t<T>::args_t;
    bool validate(const std::string& s) const 
    {
        return (convert<T>(s) <= this->values.at(0));
    }

    const std::string& to_string() const 
    {
        return "";
    }
};

template <class T>
class constraint_is_not
: public Constraint
, public args_t<T>
{
public:
    using args_t<T>::args_t;
    bool validate(const std::string& s) const 
    {
        return (convert<T>(s) != this->values.at(0));
    }

    const std::string& to_string() const 
    {
        return "";
    }
};


template <class T>
class constraint_in_range
: public Constraint
, public args_t<T>
{
public:
    using args_t<T>::args_t;
    bool validate(const std::string& s) const 
    {
        T x = convert<T>(s);
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
: public Constraint
, public args_t<T>
{
public:
    using args_t<T>::args_t;
    bool validate(const std::string& s) const 
    {
        T x = convert<T>(s);
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
ConstraintPtr AtLeast(const T& t) 
{
    return std::make_shared<constraint_at_least<T>>(t);
}

template <class T> 
ConstraintPtr AtMost(T&& t)
{
    return std::make_shared<constraint_at_most<T>>(std::move(t));
}

template <class T> 
ConstraintPtr InRange(T&& t1, T&& t2)
{
    return std::make_shared<constraint_in_range<T>>(t1,t2);
}

template <class T> 
ConstraintPtr IsNot(T&& t)
{
    return std::make_shared<constraint_is_not<T>>(std::move(t));
}

template <class T, class...TT> 
ConstraintPtr In(T first, TT...rest)
{
    return std::make_shared<constraint_in<T>>(first, rest...);
}


} // namespace clp
} // namespace elrat

#endif // include guard
