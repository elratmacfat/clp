#ifndef BUILTIN_HPP
#define BUILTIN_HPP

#include "elrat/clp/command.hpp"
#include "elrat/clp/descriptors.hpp"

#include <iostream>
#include <vector>

class HelpCommand 
: public elrat::clp::Command
{
public:
    HelpCommand(
        const std::vector<elrat::clp::DescriptorMapPtr>&,
        std::ostream* = &std::cout);
    void setOutputStream(std::ostream*);
    virtual void execute(const elrat::clp::CommandLine&);
private:
    const std::vector<elrat::clp::DescriptorMapPtr>& descriptor_maps;
    std::ostream* os;
};

class HelpDescriptor
{
public:
    static elrat::clp::CommandDescriptorPtr Create();
    HelpDescriptor() = delete;
private:
    static elrat::clp::CommandDescriptorPtr descriptor;
};

class ExitCommand
: public elrat::clp::Command
{
public:
    virtual void execute(const elrat::clp::CommandLine&);
};

class ExitDescriptor
{
public:
    static elrat::clp::CommandDescriptorPtr Create();
    ExitDescriptor() = delete;
private:
    static elrat::clp::CommandDescriptorPtr descriptor;
};

void printDescriptorMap(std::ostream&, elrat::clp::DescriptorMapPtr);

#endif

