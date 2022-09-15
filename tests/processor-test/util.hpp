#ifndef CLP_TEST_UTIL_HPP
#define CLP_TEST_UTIL_HPP

#include <elrat/clp.hpp>

elrat::clp::CommandDescriptorPtr initializeCommandDescriptor();
elrat::clp::DescriptorMapPtr initializeDescriptorMapA();
elrat::clp::DescriptorMapPtr initializeDescriptorMapB();

class Add
: public elrat::clp::Command
{
public:
    static std::shared_ptr<Add> Create();
    static const std::string& GetName();
    Add();
    bool isInitialized() const;
    double getValue() const;
    void reset();
    virtual void execute(const elrat::clp::CommandLine&);
private:
    bool initialized;
    double result;
    static const std::string name;
};

#endif

