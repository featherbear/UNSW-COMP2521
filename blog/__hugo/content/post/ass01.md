---
title: "Assignment One - Textbuffer"
date: 2018-11-30T18:33:40+11:00
categories: ["Assessments"]

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---


# Questions
Having a look at `textbuffer.h`, I was abit confused as to where the `struct textbuffer` definition was.  
_The compiler needs to know right?_  
We're not just just doing `Textbuffer __ = malloc(sizeof(*__))`; since the compiler won't know how large `struct textbuffer` is?  

EDIT: Nevermind, I did a dumb dumb; I **am** meant to implement the structure for `struct textbuffer` myself.

## Splitting the new lines.
`if (*line == '\0') break;` won't work.  
Why? Because if I had an input of `\n\nhello\n\0`, the loop would break because between `\n` and `\n` is `\0`. Aaaaeh!

## textbuffer_swap
If I switch `tb[0]`, the head does not change, and cannot change via the current function prototype.  
So then, I'll have to separate the `struct textbuffer` type from the `struct textbufferline` type.


## textbuffer_to_str
`char *array = malloc(textbuffer_bytes(tb) ? textbuffer_bytes(tb) : 1);`  

> EDIT: Overruled by FAQ  
*My textbuffer has no lines; what should textbuffer_to_str return?*  
**NULL.**

## textbuffer_bytes
This function returns the number of characters that the textbuffer represents, but does it include the `\0` (string EOF) character?  
> UPDATE: Nope it doesn't.


## textbuffer_search
This function returns the line number that contains a string.  
BUT WHAT IF DOESN'T CONTAIN THAT STRING? HALP  

Also, the `rev` parameter means to search starting from the back

> Now, you see, if counting didn't start from 0, then we could have assigned 0 to be false.  
_BUT NOOOOO WE START AT ZERO_  
**UPDATE**: `size_t` changed to `ssize_t`

## textbuffer_replace
Note to self, and to others: Don't do a while loop to keep trying to replace.
Because what if I replaced `the` to `there`?

| Hello the  
| Hello there  
| Hello therere  
| Hello thererere  
| Hello therererere  
...  
| Hello therererererererererererererererere


# More on strok vs strsep
> "You would only use strtok() when someone threatens your life if you don't do so."  
- _source: [StackOverflow](https://stackoverflow.com/a/7219504)_

# TIL
* `for` loops in C can have initialisers of **only one** data type
