# command line processor (clp)

A command line parser with additional functionality:
- Allows the application programmer to describe commands.
- A command descriptor serves two purposes. The information stored in a descriptor is used to
	- automatically generate a human readable *help* message
	- determine if an issued command line is to be considered valid.
-

## Build

- Requirements
	- Compiler support for C++11
- Optional 
	- `Boost.Test` is needed to compile the test executable.
	- `CMake` and `make` do not have to be used. The library can be compiled by hand as well.

### Manual

- Header `elrat/clp.h` must be found in the compiler's search directories, i.e. add `inc` folder.
- To create library from the object code, it must be compiled into position-independent-code (PIC).

```
g++ -c -I"inc" -fPIC -o clp.o src/clp.pp
g++ -shared -I"inc" -o libclp.so clp.o
``` 


### CMake

The root directory contains a `CMakeLists.txt` defining build targets `clp`, `unittest` and `example`.

After preparing an out-of-source build like:

```
cd /path/to/clp-root-directory
mkdir build
cd build
```

Run cmake and make to compile the targets:

``` 
cmake ..
make
```

