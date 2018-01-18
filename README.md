# JITCalculator

The plan is to create a JIT calculator. That is a Just In Time Calculator. 

This started out as an implementation of a recursive decent parser for expressions that implements operator
precedence with precedence climbing as described in this blog post: https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing.

But I decided it would be much more interesting to make it JIT.

## Compiling
If you are running macOS this is easy, just open the Xcode project and hit compile.

## Running
Currently the program will ask you for an expression to compute at the command line. Currently, it will output the x86 assembly code to
compute this expression and write it to a file under the `test/` directory.

To run the generated assembly move to the `test/` folder and run  `gcc -o program main.c compute.s` to compile, and then `./program` to
run it. 

This is obviously not a JIT, but I am working on it.

