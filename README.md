# JITCalculator

`JITCalculator` is a small functional language. 

## Data Types
Currently the only data type is Integers.

## Functions

To define a function, use the `let` keyword. If the function does not take any arguments, the parens may be omitted.

```
 let add(a,b) = a + b
 let x = 1
```

## Guards
To do any type of branching, you currently must use guards.
```
let min(a,b) 
  | a < b = a
  | else = b
```
`JITCalculator` will evaluate each guard until it finds one that evaluates to true, then it will execute and return the corresponding expression. If no guards evaluate to true, the `else` expression will be run. Currently you must have an `else` condition. Guards allow for much more interesting computations, such as ones that require looping/recursion.

## Example program
```
 let factorial(n) 
  | n > 1 = factorial(n-1) * n
  | else = 1
  
 factorial(5)
```
