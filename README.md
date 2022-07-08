# clp

## Overview

**clp** is a command line parser with additional functionality, that intends to help the programmer focussing on the actual commands, rather than getting lost in multi-level nested if-statements. This library is not only a parser, but also does the job of checking the integrity of an issued command (that's why the *p* stands for *processor* rather than *parser*).

### What does that mean?

The library offers a way to communicate the description of your commands (in a CLI-based application) to the user of your application as well as to the application itself.

The command descriptions let the user know what the individual commands do and what kind of input they expect (The output of `command --help` is generated from the description you provided.)

Additionally, the library is able to check an issued command against these descriptions, and appropriate feedback is generated. Is the command valid? If not, what's wrong?

If the issued command is considered valid, the corresponding command function can retrieved from a map, and be executed. 

All these steps are **not** encapsulated within the library. The program flow is still in the hands of the programmer. To get a better idea of how this works, consider the example application (source file: `src/example.cpp`).

Alternatively, before going into the source code, you can visit the *Implementation Details* section below.

## Build

- Requirements
	- C++11

- Optional 
	- `Boost.Test`: Just if you want to compile the test executable.
	- `CMake`: Only if you want to use the included `CMakeLists.txt`. This is not necessary at all.

### Manual build process

This is not a full-on guide on how to compile and use the library. It is assumed that the reader is able to adapt the instruction according to his/her working enviroment.

Make sure that `elrat/clp.h` can be found in the compiler's include search directory, e.g. by passing `-I"path/to/include"`. The following instructions assume the project root folder to be the current working directory, therefore `-I"inc"` points to the subdirectory `inc` where `elrat/clp.h` can be found.

```
g++ -c -I"inc" -fPIC -o clp.o src/clp.pp
g++ -shared -I"inc" -o libclp.so clp.o
``` 

An application can be linked against this library by passing `-lclp`. If necessary, specify the library search directory with `-L"path/to/lib"`.

``` 
g++ -I"inc" -lclp -o example src/example.cpp
``` 

### CMake

Included in the root directory is a `CMakeLists.txt`, which produces a shared library (or a DLL if you're on Windows), a test program, and some other executables.

```
cd /path/to/clp-root-directory
mkdir build
cd build
```

Invoke CMake on the parent directory, creating the build files in the current directory. If `Boost` could not be found, the test executable will be skipped.

```
cmake ..
make
```

### Install

After `make` had nothing to complain about, you can invoke `make install`, which copies the header and the library in their default locations. The example and test applications are not installed.

## Implementation Details

*(work in progress)*

- A command can be followed by zero or more parameters. 
- The last parameter of the command or the previous option, or previous option's parameter can be followed by another option. 
- Each option can be followed by zero or more parameters.

### Syntax:

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

