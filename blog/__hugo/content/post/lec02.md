---
title: "Lecture 2 - Abstraction"
date: 2018-11-29T10:11:01+11:00
description: "ADTs, Stacks & Queues, Analysis and Testing"
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

# Abstraction
Abstraction is the concept of taking a step back from something, and providing a wider overview.

It does **NOT** mean to be vague, but rather give a higher level of meaning.

> **Example**  
If we swap two variables with `swap(&var1, &var2)`,  
the user doesn't need to know about the `temp` variable that is needed.

## How to implement this, abstraction thing..?
There are two parts to an ADT, the <u>[interface](#interface)</u>, and the <u>[implementation](#implementation)</u>

When designing a library,  
What are the abstract properties of the datatypes we want to provide?

### Interface
An ADT's interface is the components that are exposed to the user to see and use. These are usually placed in an ADT's file header (.h file).

> **Example**  
Function prototypes: `void swap((void*)var1, (void*)var2)`  
Structure pointers: `typedef struct st *St`

_Fun Fact: The C language allows us to forward-declare function and types - We term these as prototypes_

### Implementation
An ADT's implementation is the actual inner-workings of the ADT... Helper functions, variable states, etcetera

# Uses of Abstract Data Types
Stacks and Queues are common types of ADT implementations.

## Stacks (LIFO)
> The **L**ast **I**n is the **F**irst **O**ut

### Interface
The interfaces would be the functions that a program would use

* `Stack stack_new(item_t)`
* `void stack_push(Stack, item_t)`
* `item_t stack_pop(Stack)`
* `size_t stack_size(Stack)`
* `item_t stack_peek(Stack)`

### Implementation
```c
Stack stack_new(item_t item) {
  ... malloc ...
  ... assign ... // (*stack) = (struct stack) { ... }
  ... return ...
}
void stack_push(Stack s, Item it) {
  ... add item to tail of stack ...
}
```  

## Queues (FIFO)
> The **F**irst **I**n is the **F**irst **O**ut

### Interface
* push
* pop
* size
* peek

### Implementation
`code for queue functions go here!!`


# Analysis and Testing
It's important to test our code... to make sure that it works.  
> _It's also important to check for the efficiency of our code... more on that in the [next lecture](../lec03)._

## Approaches
### TDD - Test Driven Development
Write tests for your code with expected values BEFORE you write your code.

(Often people write their code, then test it against output values... this **isn't** writing tests, as all of your tests will be false negatives)

### Unit Tests
Testing each function

### Integration Tests
Testing functions together

### Regression Tests
Testing that further development has not affected previous behaviour


> [Read more about testing on StackOverflow](https://stackoverflow.com/questions/7672511/unit-test-integration-test-regression-test-acceptance-test)


## Black Box / White Box Tests
There's also another two types of testing.

**Black Box Tests** as aptly named - you don't know what's inside the black-box, only the inputs and outputs. For these tests, we compared the output given an input.

**White Box Tests** on the other hand, is testing with knowledge of the inner workings of the code. Here we'd be checking for _structure values_, _counters_, _pointer locations_, _etcetera_.

The lecturer wrote some stuff about [Black and White Box Tests](https://blog.jashankj.space/2016-12-23-black-white-box-tests)
