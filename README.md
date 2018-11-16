# JITCalculator

`JITCalculator` is a small functional language. 

## Data Types
Currently the only data type are Integers, and List.

## List 

```
let myList = [1, 2, 3]
```
Lists may also be nested:

```
let thisisvalid = [1, [2, 3], 4, [5]]
```


## Functions

To define a function, use the `let` keyword. If the function does not take any arguments, the parens may be omitted.

```
 let add(a,b) = a + b
 let x = 1
```

## Built in List Functions
- `head` - returns the first element in the list
- `tail` - returns the whole list minus the first element
- `len` - returns the lenght of the list

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
