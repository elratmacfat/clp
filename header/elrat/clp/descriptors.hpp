#ifndef ELRAT_CLP_DESCRIPTORS_HPP
#define ELRAT_CLP_DESCRIPTORS_HPP

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <elrat/clp/commandline.hpp>
#include <elrat/clp/errorhandling.hpp>

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
    static bool EmailAddress(const std::string&);
    ParameterType() = delete;
};

template <class T> ConstraintPtr AtLeast(T&& t);
template <class T> ConstraintPtr AtMost(T&& t);
template <class T> ConstraintPtr InRange(T&& t1, T&& t2);
template <class T, class...TT> ConstraintPtr Not(T first, TT...others);
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
    static ParameterDescriptorPtr Create(
	    const std::string& name,
	    const std::string& description = "",
	    bool requirement = Mandatory,
	    TypeChecker type_checker = ParameterType::Any,
	    Constraints constraints = {});
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
    static OptionDescriptorPtr Create(
        const std::string& name,
        const std::string& description = "",
        const ParameterDescriptors& parameters = {});
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
    static CommandDescriptorPtr Create(
        const std::string& name,
        const std::string& description = "",
        const ParameterDescriptors& parameters = {},
        const OptionDescriptors& options = {});

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
    DescriptorMap(const std::string&);
    void attach(CommandDescriptorPtr);
    bool validate(const CommandLine&) const;
private:
    std::vector<CommandDescriptorPtr> descriptors;
};

//-----------------------------------------------------------------------------

class Constraint
{
public:
    virtual ~Constraint() {}
    virtual bool validate(const std::string&) const = 0;
};

template <class T, int N=0> // N = number of expected arguments, 0 = any
class AcceptArgsOfSameType
{
    static_assert( 
        !std::is_same<T,const char*>::value,
        "\n\nYou need to explicitely use 'std::string'. I can't handle 'const char*'\n");
public:

    template <class TT, class...Args>
    AcceptArgsOfSameType( const TT& first, Args...args ) {
        initializeValues( first, args... );
        if ( N ) 
            throwIf( N != values.size() );
    }

    virtual ~AcceptArgsOfSameType() {
    }

    const T& getValue(int i) const {
        return values.at(i);
    }
private:
    template <class TT, class...Args>
    void initializeValues( const TT& first, Args...args ) {
        values.push_back(first);
        return initializeValues( args... );
    }

    template <class TT>
    void initializeValues( const TT& last ) {
        values.push_back(last);
    }

    void throwIf(bool b) {
        if ( b )
            throw InitializationException(
                "Constraint parameter count mismatch.",
                std::to_string(N) 
                + " expected, but found " 
                + std::to_string(values.size()));
    }

protected:
    std::vector<T> values;
};

template <class T>
class ConstraintAtLeast
: public Constraint
, public AcceptArgsOfSameType<T,1>
{
public:
    using AcceptArgsOfSameType<T,1>::AcceptArgsOfSameType;
    bool validate(const std::string& s) const 
    {
        return (convert<T>(s) >= this->values.at(0));
    }
};

template <class T>
class ConstraintAtMost
: public Constraint
, public AcceptArgsOfSameType<T,1>
{
public:
    using AcceptArgsOfSameType<T,1>::AcceptArgsOfSameType;
    bool validate(const std::string& s) const 
    {
        return (convert<T>(s) <= this->values.at(0));
    }
};

template <class T>
class ConstraintIsNot
: public Constraint
, public AcceptArgsOfSameType<T>
{
public:
    using AcceptArgsOfSameType<T>::AcceptArgsOfSameType;
    bool validate(const std::string& s) const 
    {
        T arg{ convert<T>(s) };
        for( auto& value : this->values )
            if ( arg == value )
                return false;
        return true;
    }
};


template <class T>
class ConstraintInRange
: public Constraint
, public AcceptArgsOfSameType<T,2>
{
public:
    using AcceptArgsOfSameType<T,2>::AcceptArgsOfSameType;
    bool validate(const std::string& s) const 
    {
        T x = convert<T>(s);
        const T& t1 = this->values.at(0);
        const T& t2 = this->values.at(1);
        return ( x >= t1 && x <= t2 );
    }
};

template <class T>
class ConstraintIsIn
: public Constraint
, public AcceptArgsOfSameType<T>
{
public:
    using AcceptArgsOfSameType<T>::AcceptArgsOfSameType;
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
ConstraintPtr AtLeast(T&& t) 
{
    return std::make_shared<ConstraintAtLeast<T>>(t);
}

template <class T> 
ConstraintPtr AtMost(T&& t)
{
    return std::make_shared<ConstraintAtMost<T>>(std::move(t));
}

template <class T> 
ConstraintPtr InRange(T&& t1, T&& t2)
{
    return std::make_shared<ConstraintInRange<T>>(t1,t2);
}

template <class T, class...TT> 
ConstraintPtr Not(T first, TT...rest)
{
    return std::make_shared<ConstraintIsNot<T>>(first, rest...);
}

template <class T, class...TT> 
ConstraintPtr In(T first, TT...rest)
{
    return std::make_shared<ConstraintIsIn<T>>(first, rest...);
}


} // namespace clp
} // namespace elrat

#endif // include guard
