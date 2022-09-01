# Command Line Processor (clp)

## Introduction

Once initialized, a *command line processor* performs the following tasks.

1. Parse input command line into a uniform data format.
2. Validate the input data within the given application context.
3. Invoke the corresponding application-defined command with its validated arguments.

### 1. Parser

The parser defines and applies the syntax rules on the input command line. The library
comes with a predefined parser, named `native_parser`, but the application programmer
can provide a custom implementation meeting the desired syntax rules or performance
criteria.


### 2. Context


### 3. Invoker


### All together


