CSC 460 - Project #1
====================


Syntax of `README.md`
-------------------

`README.md` is written in the [Markdown language](http://daringfireball.net/projects/markdown/syntax).


Project Layout
--------------

To create a new project (i.e. a new OS), run `scripts/create-project` in the top level directory (on a unix machine):

    scripts/create-project my_project

The above example would create a directory name `my_project`, populated with 3 files:

*   `my_project.aps` <- an AVR Studio 4 project
*   `my_project.c` <- a C file containing the `main()` function
*   `makefile` <- a makefile which works with both the command-line and AVR Studio

Note that the generate `.aps` file is simply an XML file copied from a fresh AVR Studio
project that has been set up to use an external makefile instead of the default build system.

Here's a test change.
