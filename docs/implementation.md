# Implementation details

[Back to overview](../README.md)

### The library's default parser

The `NativeParser` is the concrete parser that comes with the library, and is deployed by default. The application programmer however can override this by providing an own parser implementation.

#### Command

The first word of a command line is interpreted as the command identifier. Unlike usual *identifiers* it may contain dashes, but must start with a letter or underscore.

Syntax examples:
```
count-words
_add_numbers
```

#### Parameter

A parameter can look like anything, but may not start with an equal sign.

Syntax examples:
```
count-words /path/to/file
_add_numbers 23 19
```

#### Option

An option must have two preceeding dashes (like `--option-name`). An option can accept exactly one parameter. An equal sign indicates the association. For example: `--diameter=15.5` (with or without spaces does not matter). If the equal sign is omitted, the parameter will be associated with the command.

Syntax examples:
```
count-words /path/to/file --word=TODO
```

#### Option-Pack

One or more 1-character long options (that don't accept parameters) can be packed together. For example `-abc` will set three different options (`a`,`b` and `c`).
Syntax examples:
```
tar -cvf
```

#### State machine 

![](img/native-parser-state-machine.png)