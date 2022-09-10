#ifndef CLP_TEST_UTIL_HPP
#define CLP_TEST_UTIL_HPP

#include <elrat/clp.hpp>

elrat::clp::DescriptorMapPtr createDescriptorMap();

class Add
: public elrat::clp::Command
{
public:
    static std::shared_ptr<Add> Create();
    Add();
    bool isInitialized() const;
    double getValue() const;
    void reset();
    virtual void execute(const elrat::clp::CommandLine&);
private:
    bool initialized;
    double result;
};

#endif

