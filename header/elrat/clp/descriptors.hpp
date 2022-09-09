#ifndef ELRAT_CLP_DESCRIPTORS_HPP
#define ELRAT_CLP_DESCRIPTORS_HPP

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <elrat/clp/commandline.hpp>

namespace elrat {
namespace clp {

class DescriptorMap;
using DescriptorMapPtr = std::shared_ptr<DescriptorMap>;

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

using Argument = std::string;
using Arguments = std::vector<std::string>;

extern const bool Mandatory;
extern const bool Optional;

class ParameterType 
{
public:
    static bool Any(const std::string&);
    static bool NaturalNumber(const std::string&);
    static bool WholeNumber(const std::string&);
    static bool RealNumber(const std::string&);
    static bool Name(const std::string&);
    static bool Identifier(const std::string&);
    static bool Path(const std::string&);
    ParameterType() = delete;
};

CommandDescriptorPtr makeCommandDescriptor(
    const std::string& name,
    const std::string& description = "",
    const ParameterDescriptors& parameters = {},
    const OptionDescriptors& options = {}
);

OptionDescriptorPtr makeOptionDescriptor(
    const std::string& name,
    const std::string& description = "",
    const ParameterDescriptors& parameters = {}
);

ParameterDescriptorPtr makeParameterDescriptor(
    const std::string& name,
    const std::string& description = "",
    bool requirement = Mandatory,
    TypeChecker type_checker = ParameterType::Any,
    Constraints constraints = {}
);

template <class T> ConstraintPtr AtLeast(const T& t);
template <class T> ConstraintPtr AtMost(T&& t);
template <class T> ConstraintPtr InRange(T&& t1, T&& t2);
template <class T> ConstraintPtr Not(T&& t);
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

class HasParameters
{
public:
    HasParameters(const ParameterDescriptors&);
    HasParameters(ParameterDescriptors&&);
    const ParameterDescriptors& getParameters() const;
    int getRequiredParameterCount() const;
    void validate(const Arguments&) const;
protected:
    ParameterDescriptors parameters;
    int numberOfRequiredParameters;

    void initialize();
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
    void validate(const Argument&) const;
private:
    bool        required;
    TypeChecker type_checker;
    Constraints constraints;
};

//-----------------------------------------------------------------------------

class OptionDescriptor 
: public HasName
, public HasDescription
, public HasParameters
{
public:
    OptionDescriptor(
        const std::string&,
        const std::string&,
        const ParameterDescriptors& );
    const ParameterDescriptors& getParameters() const;
    bool validate(const Argument&, const Arguments&) const;
};

//-----------------------------------------------------------------------------

class CommandDescriptor
: public HasName
, public HasDescription
, public HasParameters
{
public:
    CommandDescriptor(
        const std::string&,
        const std::string&,
        const ParameterDescriptors&,
        const OptionDescriptors& );
    
    const OptionDescriptors& getOptions() const;
    
    bool validate( const CommandLine& ) const;
private:
    OptionDescriptors       options;
};

//-----------------------------------------------------------------------------

class DescriptorMap
: public HasName
{
public:
    static DescriptorMapPtr Create(const std::string& = "Commands");
    void attach(CommandDescriptorPtr);
    bool validate(const CommandLine&) const;
private:
    DescriptorMap(const std::string&);
    std::vector<CommandDescriptorPtr> descriptors;
};

//-----------------------------------------------------------------------------

class Constraint
{
public:
    virtual ~Constraint() {}
    virtual bool validate(const std::string&) const = 0;
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
    T getValue(int i) 
    {
        return values.at(i);
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
ConstraintPtr Not(T&& t)
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
