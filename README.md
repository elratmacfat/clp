# elrat-clp

(Work in progress)

C++ library

## Overview

**elrat-clp** is a command line parser with additional functionality, that intends to help the programmer focussing on the actual commands, rather than getting lost in multi-level nested if-statements. This library is not only a parser, but something like a processor, a **command line processor**.

### What does that mean?

The library offers a way to communicate the description of your commands (in a CLI-based application) to the user of your application as well as to the application itself.

The command descriptions let the user know what the individual commands do and what kind of input they expect (The output of `command --help` is generated from the description you provided.)

Additionally, the library is able to check an issued command against these descriptions, and appropriate feedback is generated. Is the command valid? If not, what's wrong?

If the issued command is considered valid, the corresponding command function can retrieved from a map, and be executed. 

All these steps are **not** encapsulated within the library. The program flow is still in the hands of the programmer. To get a better idea of how this works, consider the example application (source file: `src/example.cpp`).

Alternatively, before going into the source code, you can visit the *Implementation Details* section below.


### How does it compile?

It requires C++11.

#### CMake

Included in the root directory is a `CMakeLists.txt`, which produces a shared library (or a DLL if you're on Windows), a test program, and some other executables.

The test program depends on `elrat-testbench` (a header-only library). If it is not found, compiling the test will be skipped.

#### Manual
- Copy sources into your project directly
    - Copy `src/clp.cpp` and `inc/elrat/clp.h` to your project.
- Compile into a library manually
    - *TODO*

## Implementation Details

- A command can be followed by zero or more parameters. 
- The last parameter of the command or the previous option, or previous option's parameter can be followed by another option. 
- Each option can be followed by zero or more parameters.

#### Syntax:

```
<Command> <Parameter>* [<--Option|-O> <Parameter>*]*
    < >     Value of command, option or parameter.
    *       0 or more
    --      Option name. The double dash is followed by an option name of at 
            least one character.
    -       Option shorthand. A single dash is followed by exactly one character.
            The next characters will be treated as the option's parameter, e.g.
            -d5 is the same as -d 5 or -d=5.
```

The `inspect_cmdstr` application (source file: `src/inspect_cmdstr.cpp`) provides a quick way to see
how an issued command line would be interpreted.

