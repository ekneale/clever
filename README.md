##To build

$ cmake -B build

$ cmake --build build

// To clean first
$ cmake --build build --target clean

##Structure of the code

The class libraries are stored in the libclever directory, with the naming
convention lib[classname].cpp. Headers are stored in the same directory with
the same naming convention.

The main executable and peripherals are stored in the clever directory.


##Unit tests

Unit tests for each class have been built in to the code and have the naming 
convention lib[classname].test.cpp.

To run all unit tests after build do:
$ ./build/cleverTests

##C++ Style Guidelines

The following style protocol has been used and should be followed as closesly as
possible for changes/additions to the code.

Class names and method names should be written in CamelCase (acronyms can stay all upper case).
Class member variables should be written in CamelCase, but with an initial "m".
Local variable names should be written in lowerCamelCase.

Avoid arrays - prefer STL vectors. Vectors can be variable size, re-sized, passed by reference, etc.

Avoid pointers when you can:
Pass by reference (e.g. int foo (MyObject &obj).



Order of functions in cpp file, and order of declaration in hpp file:

1. Constructors & Destructors
2. Public methods - ordered by importance
3. Protected methods - ordered by importance
4. Protected members
5. Private methods
6. Private members
(Prefer private over protected - only assign as protected if necessary)

Prefixes:

m for members
c for constants
p for pointer (pp for pointer to pointer)
s for static
i for indices and iterators

Suffixes for type:

List, Vector, etc.

E.g. itemList[iItem] = pItem.



##LICENCE

Clever is released under the OSI-approved [3-Clause BSD Licence](https://opensource.org/licenses/BSD-3-Clause).
