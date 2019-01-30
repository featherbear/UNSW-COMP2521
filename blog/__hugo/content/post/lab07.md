---
title: "Lab 7 - Quick Sort Detective"
date: 2019-01-28T19:56:24+11:00

hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

> **Resources:**  
- [GitHub](https://github.com/featherbear/UNSW-COMP2521/tree/gh-pages/Labs/lab07) (Sort code, data.py)  
- [Timing Data](Timings.xlsx)  
- [Raw Timing Data](https://github.com/featherbear/UNSW-COMP2521/tree/gh-pages/blog/__hugo/static/post/lab07) (CSV files)  

# Overview  

* Pick a **pivot** element.
* **Partition** the array into 3 parts:
  * First part: all elements in this part is less than the pivot.
  * Second part: the pivot itself (only one element!)
  * Third part: all elements in this part is greater than or equal to the pivot.
* Then, apply the quicksort algorithm to the first and the third part. (recursively)  
[Source: me.dt.in.th](http://me.dt.in.th/page/Quicksort/)

# Scripts
In analysing an algorithm's performance, we should always consider the average case.  
For sorting, this would mean a randomised array of values.

Since I was conducting my tests on a non-CSE computer, I can't use the [`./gen` program provided in lab06](../lab06/#gen) _(I mean it's not like I used it last time either)_.  

## RNG
```bash
python3 -c 'from random import shuffle; (lambda s: [print(s), [print(i) for i in (lambda a: shuffle(a) or a)(list(range(s)))]])(100000)'
```  
Have I told you I love my one-liner scripts :)  

You can't do multiline statements in lambda functions, nor can you use the semicolon to separate statements... Instead we can execute multiple functions by putting them in an array.  

## More RNG
Or I could repurpose my [sort detective helper script](../lab06/#lots-of-data-apos-lazily-apos).
```python3
#!/bin/python3
import sys

import subprocess
from random import shuffle

# Returns the mean of a list of items (to 3 decimal places)
def average(items):
    sum = 0
    for item in items:
        sum += item
    return round(sum / len(items), 3)

n = 100
while True:
    print(str(n) + ",", file=sys.stderr, end=" ")

    nums = list(range(n))                              # Ascending
    if sys.argv[1] == "D": nums = nums[::-1]           # Descending

    times = []
    for x in range(10):
        
        # Fixed from last time, where the shuffled order was always the same!
        if sys.argv[1] == "R": shuffle(nums)           # Random
        string = "\n".join(map(str, [n] + nums)).encode()
        
        with subprocess.Popen(["/usr/bin/time", "-f%U", "./quicksort", "-p<INSERTYOURMODEFLAGHERE>", "-q"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE) as process:
            comm = process.communicate(string)
            times.append(float(comm[1].decode().strip()))

    avg = average(times)
    print(str(n) + ",", avg)
    print(avg, file=sys.stderr)

    if avg > 1.0: break
    if n == 500000: break

    if n < 10000: n += 100
    else: n += 2000
```

This script will generate the timings for lots of numbers (_0-10000 in increments of 100, then 10000-500000 in increments of 2000_), and also take the average time over 10 sorts.

# Data
[![](dataSnippet.png)](Timings.xlsx)  
[Download Timings](Timings.xlsx)  

# Analysis
![](Snipaste_2019-01-28_20-02-34.png)
***Note: The x-axis increases by increments of 100, and then increments of 2000 after 10000 items***  
&nbsp;  

This graph was made from the timings of a randomised array from each of the sort methods.

The graph above suggests that all three sorts perform very similarly.

In the data set, the <span style="color:royalblue">random</span> method was more performant than that of the <span style="color:red">naive</span> and <span style="color:forestgreen">median</span> method.  

This could be as a result of the <span style="color:red">naive</span> and <span style="color:forestgreen">median</span> methods requiring more operations _(nature of a <span style="color:red">naive</span> sort, and to compare and switch values for a <span style="color:forestgreen">median</span> sort)_

The timings for the <span style="color:forestgreen">median</span> method were (surprisingly) more inconsistent, than the <span style="color:red">naive</span> sort, evident in the visible timing spikes

For a dataset of about 364,000 items, both the <span style="color:red">naive</span> and <span style="color:forestgreen">median</span> sort methods spiked up in sort times

_// Aside: It would have been better to perform the same tests with the same randomised numbers_

## Naive Pivot
![](Snipaste_2019-01-28_19-55-08.png)  
***Note: The x-axis increases by increments of 100, and then increments of 2000 after 10000 items***  
&nbsp;  

This algorithm uses the left/right-most value of an array as the pivot index.  
It is naive as there is no thought given into which value to pick as the pivot.

This algorithm reaches its worst-case time complexity when sorting an already sorted array.

## Median of Three Pivot
![](Snipaste_2019-01-28_19-55-41.png)  
***Note: The x-axis increases by increments of 100, and then increments of 2000 after 10000 items***  
&nbsp;  

The median of three pivot algorithm compares the first, middle and last values of an array, and uses the median value as the pivot index - whilst also sorting the three values (lo, mi, hi) in place.  This poses the advantage of the sort always pivoting around a value where there are most likely lesser and greater values.

Sorting a randomised array of numbers takes (expectedly) more time than an already sorted ascending array.  

The performance of a median sort on descending items is interesting - as in my head it should perform similar to a randomised array.  
Overall the median method has a better overall sort performance than the naive sort, which bottlenecks for an (as/des)cending array.

## Random Pivot
![](Snipaste_2019-01-28_19-56-05.png)  
***Note: The x-axis increases by increments of 100, and then increments of 2000 after 10000 items***  
&nbsp;  

Albeit not knowing the nature of the values in an array, the random pivot method generates the best average case operation.  
This algorithm selects a random value in an array as the pivot index.  
It can sort an array of 500,000 random values in roughly `0.11s` (on my laptop at least).  

For a quick sort on an (as/des)cending array of numbers), the random pivot sort takes slightly longer than a median method - which is to be expected as there is no thought given into picking a pivot, hence on average more partitions will be performed.  
Like the median sort, the random pivot sort is faster than the naive sort

# Gotta go faster!
By implementing a different sorting algorithim within another algorithm, for instance an Insertion Sort within the Quick Sort - we can optimise the performance of our sort. Such is because an Insertion Sort may perform fewer comparisons, swaps and recursions (for a small number of values).

To later compare... for a randomised array of 1,000,000 values, it takes an unoptimised/non-hybrid Quick Sort an average of `0.206s` to sort.

## In-place Insertion Sort
If a partition is smaller than a threshold `M`, then an Insertion Sort may be performed instead of the Quick Sort for that partition.

> "switch to insertion sort for partitions smaller than the threshold M"

_For a randomised array of 1,000,000 values..._  
`values -> /usr/bin/time --format="%U" ./quicksort -pr -q -sa<THRESHOLD>`  

|Threshold (M)|Avg|1|2|3|4|5|
|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
|0|0.208|0.21|0.21|0.20|0.21|0.21|
|1|0.210|0.20|0.21|0.21|0.21|0.22|
|2|0.208|0.21|0.21|0.21|0.20|0.21|
|3|0.208|0.22|0.20|0.20|0.21|0.21|
|4|0.200|0.20|0.19|0.19|0.20|0.22|
|5|0.202|0.20|0.20|0.20|0.20|0.21|
|6|0.200|0.20|0.20|0.20|0.20|0.20|
|7|0.198|0.20|0.20|0.20|0.19|0.20|
|8|0.192|0.19|0.19|0.19|0.19|0.20|
|**9**|**0.184**|**0.18**|**0.18**|**0.18**|**0.19**|**0.19**|
|10|0.196|0.20|0.19|0.19|0.20|0.19|
|100|0.196|0.20|0.19|0.20|0.19|0.20|
|1000|0.442|0.46|0.45|0.43|0.44|0.43|
|10000|3.208|3.38|3.27|3.16|3.06|3.17|

This optimisation (threshold `M=9`) shaves off `0.206 - 0.184 = 0.022s` of execution time.

## Deferred Insertion Sort
Another way to reduce the number of insertion sorts performed is to do just one Insertion Sort at the end of all Quick Sort operations.

> "do not sort partitions smaller than the threshold M; leave them unsorted, and do an insertion sort at the end"

_For a randomised array of 1,000,000 values..._  
`values -> /usr/bin/time --format="%U" ./quicksort -pr -q -sb<THRESHOLD>`  

|Threshold (M)|Avg|1|2|3|4|5|
|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
|0|0.206|0.20|0.21|0.20|0.21|0.21|
|1|0.208|0.21|0.21|0.21|0.20|0.21|
|2|0.206|0.20|0.21|0.21|0.21|0.20|
|3|0.206|0.21|0.21|0.21|0.19|0.21|
|4|0.204|0.21|0.19|0.20|0.21|0.21|
|5|0.198|0.20|0.20|0.20|0.19|0.20|
|6|0.194|0.20|0.20|0.19|0.19|0.19|
|7|0.198|0.20|0.20|0.20|0.20|0.19|
|8|0.200|0.20|0.20|0.20|0.20|0.20|
|9|0.198|0.20|0.19|0.20|0.20|0.20|
|10|0.188|0.19|0.19|0.18|0.19|0.19|
|**100**|**0.186**|**0.19**|**0.19**|**0.18**|**0.18**|**0.19**|
|1000|0.440|0.42|0.44|0.44|0.43|0.47|
|10000|3.186|3.31|3.07|3.27|3.05|3.23|

This optimisation (threshold `M=100`) shaves off `0.206 - 0.186 = 0.020s` of execution time.
	
# Appendix
![](Snipaste_2019-01-28_20-02-34.png)

---

![](Snipaste_2019-01-28_19-55-08.png)  

---

![](Snipaste_2019-01-28_19-55-41.png)  

---

![](Snipaste_2019-01-28_19-56-05.png)  


