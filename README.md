# cflatc

cflatc is a compiler for the fictional language Cflat. It was built as a final
project for the CIS4650W15 Compilers class at the University of Guelph.

cflatc can take Cflat programs and convert them into MIPS assembly. It can also
output intermediate representations in the form of an abstract syntax tree, a
symbol table, and three address code.

## Building

Run `make` to compile the `cflatc` program into the `build` directory.

Run `make test` to compiler the `test_cflatc` into the `build` directory.

## Usage

`cflatc` takes several optional arguments:

    $ bin/cflatc --help
    Usage: cflatc [OPTION...] [FILE]
    cflatc - compile Cflat programs
    cflatc reads from the named input file (or standard input if no file is named.
    http://www.uoguelph.ca/~dmccaugh/teaching/CIS4650/Project/

        -a, --abstract          Perform syntax analysis and output abstract syntax (.abs)
        -c, --compile           Compile and output MIPS R2000 assembler (.asm)
        -i, --intermediate      Perform sematic analysis and output intermediate representation (.inr)
        -o, --output=FILE       Output results to FILE
        -s, --symbol            Perform type checking and output symbol table (.sym)
        -?, --help              Give this help list 
        --usage                 Give a short usage message
        -V, --version           Print program version

        Mandatory or optional arguments to long options are also mandatory or
        optional for any corresponding short options.

        Report bugs to rwilsonp@mail.uoguelph.ca.
