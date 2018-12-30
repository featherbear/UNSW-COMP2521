---
title: "Lecture 3 - Algorithms"
date: 2018-12-04T14:14:06+11:00
description: "Complexity, Recursion, Fibonacci"
categories: ["Lectures"]

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

# Complexity
## Factors that affect good programs
### Environment
* the Operating System
* the Compiler
* the Hardware
### Implementation
* the Algorithm (our code)

## Testing
> Empirical testing of the efficiency of algorithms
* Choose a good set of inputs
* Measure the actual runtime (environment is important)

## Timing
### Environmental factors
As different environments will produce different absolute times, we want to focus on the relative changes

### Time Complexity
When calculating the time complexity of an algorithm, we consider the worst case scenario.  
_(ie for a search algorithm; key not in A)_

#### Big-O Notation
> From best to worst timing

Constant - `O(1)`  
Logarithmic - `O(log n)`  
Linear - `O(n)`  
n-log-n - `O(n log n)`  
Quadratic - `O(n^2)`  
Cubic - `O(n^3)`  
~~ fac - `O(n!)`  
~~ Exponential - `O(2^n)`  



The worst case execution time is between the fastest and slowest possible operation time.
[Searching Algorithms](../searchingAlgorithms)

#### Calculating Big-O Values
> Remember: The Big-O value refers to the worst-case scenario  

Step through the algorithm and count the number of operations performed. When you encounter a loop, add `n`, or a variant of `n`.

The resulting Big-O is the highest power of n, expressed without the coefficient.

> Example :: 4 + **<u>3n</u>**  
Highest power of n: 3n  
Big-O: O(n) - linear  

> Example :: 4 + 3n + **<u>3n^2</u>**  
Highest power of n: 3n^2  
Big-O: O(n^2) - quadratic  


##### Maths-y stuff
f(n) is O(g(n)) if f(n) is asymptotically less than or equal to g(n).  
f(n) is Omega(g(n)) if f(n) is asymptotically greater than or equal to g(n).  
f(n) is Theta(g(n)) if f(n) is asymptotically equal to g(n).

// Given f(n) and g(n), we say f(n) is O(g(n)) if we have positive constants c and n_0 

**Tractable** - polynomial-time algorithm; polynomial worst-case performance O(n^2)  
useful and usable in in practical applications

**Intractable** - No algorithm exists (usually NP')  
Worse than polynomial performance O(n^2)
Feasible only for small n

**Non-computable** -- no algorithm exists (or can exist)

### Tools
#### _time(1)_
measure execution time  
Also available at `/usr/bin/time`

# Recursion
## Components
* Base Case - aka our stopping case
* Recursive Case - will call itself

## Calculating Factorials
### Iterative Approach
```c
int fac(int n) {
  int result = 1;
  for (int i = 1; i <= n; i++) result *= i;
  return result;
```
### Recursive Approach
```c
int fac(int n) {
  if (n == 1) return 1;
  return n * fac(n-1);
```

To calculate `fac(3)` the program will need to do
```js
1 | fac(4) = 4 * fac(3)
2 |              fac(3) = 3 * fac(2)
3 |                           fac(2) = 2 * fac(1)
4 |                                        fac(1) = 1
```
That's a lot of stack frames that it needs to set up and destroy...  
Would be better to just iterate for a factorial function

## Fibonacci
_So this section alone got quite large..._
> [Click here to read the Fibonacci post](../c-fibonacci)

## Optimisation
Tail-call optimisation: