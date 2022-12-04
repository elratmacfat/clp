#ifndef CLP_TEST_UTIL_HPP
#define CLP_TEST_UTIL_HPP

#include "elrat/clp/command.hpp"
#include "elrat/clp/descriptors.hpp"

elrat::clp::DescriptorMapPtr initializeDescriptorMapA();
elrat::clp::DescriptorMapPtr initializeDescriptorMapB();

class Add
: public elrat::clp::Command
{
public:
    static elrat::clp::CommandDescriptorPtr GetDescriptor();
    static std::shared_ptr<Add> CreateCommand();
    Add();
    bool isInitialized() const;
    double getValue() const;
    void reset();
    virtual void execute(const elrat::clp::CommandLine&);
private:
    bool initialized;
    double result;
    static const std::string name;
    static elrat::clp::CommandDescriptorPtr descriptor;
};

#endif

