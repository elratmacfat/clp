# Command Line Processor (clp)

## What is it?

Reading a command line from some source (a user-interface, a script file, ...) usually is followed by these three steps

1. Parse input into logical pieces
1. Check if pieces satisfy syntax rules and other constraints
1. Invoke an appropriate function that works with the input data.

The library aims to provide a simple way for the application programmer to describe and define application-specific commands. Once everything is set up, the incoming command lines can be forwarded to the command line processor, which then parses and validates the input and invokes the corresponding input handler. 

![](docs/img/top-level-principle.png)

## Build

Requirements

- Compiler support for `C++11`
- `CMake` for the automated build process
    - version 3.10 or later
- `Boost Unit Test Framework` for the test executables
    - version 1.74 is used, but older version are likely to be sufficient as well

## Getting started

As soon as the library reaches a usable state, this part of the documentation will be addressed.

## Implementation details

Some information regarding the implementation can be found [here](docs/implementation.md)


