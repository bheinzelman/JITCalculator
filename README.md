# JITCalculator

The plan is to create a JIT calculator. That is a Just In Time Calculator. 

This started out as an implementation of a recursive decent parser for expressions that implements operator
precedence with precedence climbing as described in this blog post: https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing.

But I decided it would be much more interesting to make it JIT.

## Compiling
If you are running macOS this is easy, just open the Xcode project and hit compile.

## Running
Currently the program will ask you for an expression to compute at the command line. The idea is that eventually the program will 
generate machine code on the fly, but currently it will just print some asm to stdout. 
