# JITCalculator

`JITCalculator` is a small functional language. 

## Data Types
Currently the only data types are Integers, List, and Strings.

## List 

```
let myList = [1, 2, 3]
```
Lists may also be nested:

```
let thisisvalid = [1, [2, 3], 4, [5]]
```

Lists may be indexed:

```
# get the 3rd element
let myList = [1,2,3,4]
myList()[2]
```

Lists may be sliced (just like python):

```
let myList = [1,2,3,4]

# returns [2,3]
myList()[1:3]

# returns [2,3,4]
myList()[1:]

# returns [1,2,3]
myList()[:3]

# returns [1,2,3,4]
myList()[:]

```

Strings may be treated as lists, i.e. you can index, and slice strings.

## Functions

To define a function, use the `let` keyword. If the function does not take any arguments, the parens may be omitted. In jc functions are first class, meaning they can be passed around as variables/arguments to methods.

```
 let add(a,b) = a + b
 let x = 1
```

## Built in List Functions
- `head` - returns the first element in the list.
- `tail` - returns the whole list minus the first element.
- `len` - returns the length of the list.
- `isEmpty` - returns if the list is empty.

All list functions will also work with Strings.

## Guards
To do any type of branching, you currently must use guards.
```
let min(a,b) 
  | a < b = a
  | else = b
```
`JITCalculator` will evaluate each guard until it finds one that evaluates to true, then it will execute and return the corresponding expression. If no guards evaluate to true, the `else` expression will be run. Currently you must have an `else` condition. Guards allow for much more interesting computations, such as ones that require looping/recursion.

## Closures

Closures are functions that capture scope and may be defined on the fly. 

```
# closure that takes one argument
# returns 2
{ (x) = x + 1 }(1)
```

```
# example capturing scope
# returns 4

let addThem(x,y) = { () = x + y }()
addThem(2,2)
```

## Example program
```
 let factorial(n) 
  | n > 1 = factorial(n-1) * n
  | else = 1
  
 factorial(5)
```
