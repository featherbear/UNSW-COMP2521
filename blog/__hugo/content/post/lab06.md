---
title: "Lab 6 - Sort Detective Report"
date: 2019-01-15T18:19:48+11:00

description: "A compulsory voluntarial analysis and algorithm identification of two programs of unknown sorting algorithm"
hiddenFromHomePage: false
postMetaInFooter: false

flowchartDiagrams:
  enable: false
  options: ""

sequenceDiagrams: 
  enable: false
  options: ""

---

> Lol well this was all wrong xd but das okay

# Background
## A good algorithm
* Values are only compared where needed
* Quick speed / low execution counts
* Deterministic output
* [Sort stability](#sorting-stability)

## Sorting algorithms
* **Bubble sort (unoptimised)**
* **Bubble sort (optimised)**
* **Insertion sort**
* **Selection sort**
* **Shell sort (Powers of 4)**
* **Shell sort (Sedgewick)**
* **Pseudo-Bogo Sort**

## Complexity
|Sort|Best Case|Average Case|Worst Case|
|:--:|:--:|:--:|:--:|
|Bubble|O(n)|O(n^2)|O(n^2)|
|Insertion|O(n)|O(n^2)|O(n^2)|
|Selection|O(n^2)|O(n^2)|O(n^2)|
|Shell|O(n logn)|O(n logn^2)|O(n logn^2)|
|Shell (Sedgewick)|O(n logn)|O(n logn^2)|O(n^4/3)|
|Bogo|-|-|-|

## Sorting Stability
A sorting implementation is considered ***stable*** if values maintain ordering when sorted by another key.  

For example

|Item|Key A|Key B|
|:--:|:---:|:---:|
| a | 200 | 1 |
| d | 300 | 2 |
| b | 200 | 1 |
| c | 200 | 1 |

Would return

|Item|Key A|Key B|
|:--:|:---:|:---:|
| a | 200 | 1 |
| b | 200 | 1 |
| c | 200 | 1 |
| d | 300 | 2 |

Rather than some variant where the a -> b -> c ordering is not preserved

---

|Stable|Unstable|
|:-----|:-------|
|**1 C**<br>**1 A**<br>**1 B**<br>2<br>3<br>4<br>5<br><br><br>|**1 A**<br>**1 B**<br>**1 C**<br>2<br>3<br>4<br>5<br><br>_or some variation thereof_|

A stable sort would keep the ordering (_for key 1_) **CAB**,  
whereas an unstable sort might produce **ABC**, **ACB**, **BAC**, **BCA**, or **CBA**

# Design
The program execution time of `sortA` and `sortB` can be used to analyse the type of algorithm implemented.
For example, a bubble sort in its best case (already sorted) scenario should occupy O(n) time

## Methodology
> **Linear Time**  
Traversing a sorted array of `2n` elements should take twice the time of a sorted array of `n` elements
&nbsp;  
> **Bubble vs Insertion**  
Both Bubble Sort and Insertion Sort share the same best, average and worst case execution times. 
However one input case where [Insertion Sort is faster than Bubble Sort](https://cs.stackexchange.com/questions/52461/differentiating-between-bubblesort-and-insertionsort#comment109732_52463) is {1,3,3,3,3,3,...,3,2}. Where Insertion Sort will take `O(n)` time while Bubble Sort would take `O(n^2)` time.  
_We can craft an input with:_ `python3 -c "print('\n'.join(map(str,[1,*([3]*(1000000-2)),2])))"`  
&nbsp;  
> **Checking for stability**  
We can craft an input to check for stablity
```
1 C
2
1 A
3
1 B
4
5
```
> **Checking for bogo-ness**  
For a sort of the same array of numbers taken `n` times, if the differences between timings are considerably different, or if the sort never finishes within a reasonable time - then it is highly liking that the sort is a bogo sort.  


## Lots of data ... &apos;lazily&apos;
> I can't be bothered to learn bash scripting so I'll just use Python

This script generates a comma-separated list of `n_items, time`.  
This could be used for finding patterns, or even for graphing/plotting purposes  

Usage: `./data.py [items] [A/D/R]`  

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

for n in range(2, int(sys.argv[1]), 10000):
  print(n, file=sys.stderr, end=" ")
  if sys.argv[2] == "D": nums = range(n, -1, -1)   # Descending
  else: nums = range(n)                            # Ascending
  
  nums = list(nums)
  if sys.argv[2] == "R": shuffle(nums)             # Random
  string = "\n".join(map(str,nums)).encode()

  times = []
  for x in range(10):
    with subprocess.Popen(["/usr/bin/time", "-f%U", "./sortA"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE) as process:
      times.append(float(process.communicate(string)[1][1:-2]))

  avg = average(times)
  print(str(n) + ",", avg)
  print(avg, file=sys.stderr, end="")
```


## Tools
### gen
> _Although I didn't use this because my data generation script generated the numbers for me already_

`./gen <n-values> (A|D|R) [seed]                      `  

`./gen` is a program that generates numbers either in ascending, descending or random order  
_(n-values is actually n+1 values but sure)_


### GNU time
_Version: 1.7_  
`/usr/bin/time --format="%U"`  

The `time` program measures the execution time of a program.  
_We are only interested in the "user execution time" of our programs, so we can add the flag `--format="%U%"`_


### Experimentation
All tests would be conducted 10 times, and the average of these timings would be taken to mitigate time inconsistencies caused by environmental factors (computer load, other users, things gone wrong)

The tests would also be run on the same UNSW CSE computer (wagner), under the same user account

# Results
## sortA
|Numbers|Avg|1|2|3|4|5|6|7|8|9|10|
|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
|**Ascending**|
|500000 A|0.127|0.12|0.12|0.13|0.13|0.13|0.13|0.11|0.13|0.14|0.13|
|1000000 A|0.262|0.26|0.26|0.25|0.28|0.26|0.27|0.26|0.26|0.27|0.25|
|**Descending**|
|500000 D|0.179|0.17|0.18|0.18|0.19|0.18|0.18|0.18|0.18|0.17|0.18|
|1000000 D|0.395|0.41|0.39|0.39|0.40|0.36|0.42|0.38|0.39|0.40|0.41|
|**Random**|
|500000 R|0.383|0.38|0.38|0.39|0.39|0.38|0.38|0.39|0.37|0.38|0.39|
|1000000 R|0.808|0.82|0.81|0.81|0.80|0.82|0.80|0.82|0.81|0.80|0.79|

|echo -e &quot;1 C\n2\n1 A\n3\n1 B\n4\n5&quot; \| ./sortA|
|:--|
|1 C<br>1 A<br>1 B<br>2<br>3<br>4<br>5<br>|
 
**Further Timing**

|# Items|Ascending|Descending|Random|
|--:|:--:|:--:|:--:|
|2|0.000|0.000|0.000|
|10002|0.000|0.000|0.000|
|20002|0.000|0.000|0.010|
|30002|0.000|0.000|0.010|
|40002|0.000|0.010|0.020|
|50002|0.010|0.010|0.028|
|60002|0.009|0.019|0.030|
|70002|0.010|0.019|0.040|
|80002|0.015|0.020|0.048|
|90002|0.019|0.020|0.057|
|100002|0.020|0.029|0.059|
|110002|0.020|0.029|0.068|
|120002|0.026|0.038|0.074|
|130002|0.024|0.040|0.079|
|140002|0.026|0.040|0.087|
|150002|0.032|0.045|0.098|
|160002|0.037|0.049|0.103|
|170002|0.036|0.056|0.114|
|180002|0.038|0.060|0.119|
|190002|0.040|0.059|0.129|
|200002|0.045|0.064|0.135|
|210002|0.045|0.066|0.142|
|220002|0.048|0.068|0.148|
|230002|0.050|0.078|0.160|
|240002|0.055|0.077|0.164|
|250002|0.055|0.081|0.174|
|260002|0.059|0.089|0.180|
|270002|0.062|0.090|0.185|
|280002|0.066|0.090|0.198|
|290002|0.065|0.096|0.209|
|300002|0.072|0.100|0.209|
|310002|0.071|0.106|0.218|
|320002|0.072|0.110|0.229|
|330002|0.079|0.119|0.238|
|340002|0.081|0.116|0.243|
|350002|0.081|0.119|0.252|
|360002|0.086|0.121|0.261|
|370002|0.088|0.125|0.267|
|380002|0.091|0.134|0.274|
|390002|0.095|0.132|0.288|
|400002|0.098|0.140|0.297|
|410002|0.096|0.142|0.297|
|420002|0.093|0.149|0.310|
|430002|0.102|0.149|0.315|
|440002|0.102|0.155|0.330|
|450002|0.109|0.158|0.332|
|460002|0.108|0.161|0.337|
|470002|0.112|0.168|0.347|
|480002|0.112|0.172|0.357|
|490002|0.117|0.171|0.363|
|500002|0.121|0.178|0.381|
|510002|0.121|0.184|0.383|
|520002|0.128|0.190|0.392|
|530002|0.126|0.195|0.399|
|540002|0.127|0.197|0.405|
|550002|0.136|0.189|0.419|
|560002|0.132|0.206|0.423|
|570002|0.137|0.201|0.435|
|580002|0.139|0.205|0.441|
|590002|0.141|0.211|0.449|
|600002|0.148|0.221|0.455|
|610002|0.149|0.227|0.460|
|620002|0.151|0.227|0.468|
|630002|0.155|0.228|0.481|
|640002|0.153|0.239|0.494|
|650002|0.158|0.238|0.496|
|660002|0.157|0.244|0.508|
|670002|0.159|0.246|0.514|
|680002|0.160|0.248|0.528|
|690002|0.169|0.253|0.528|
|700002|0.166|0.266|0.541|
|710002|0.170|0.273|0.546|
|720002|0.173|0.275|0.554|
|730002|0.180|0.285|0.562|
|740002|0.181|0.281|0.579|
|750002|0.186|0.284|0.575|
|760002|0.183|0.287|0.598|
|770002|0.185|0.287|0.604|
|780002|0.193|0.298|0.629|
|790002|0.191|0.300|0.627|
|800002|0.192|0.309|0.622|
|810002|0.196|0.307|0.633|
|820002|0.200|0.317|0.648|
|830002|0.206|0.309|0.654|
|840002|0.199|0.328|0.661|
|850002|0.203|0.324|0.671|
|860002|0.212|0.337|0.680|
|870002|0.208|0.336|0.703|
|880002|0.216|0.336|0.715|
|890002|0.219|0.356|0.722|
|900002|0.221|0.348|0.720|
|910002|0.216|0.352|0.729|
|920002|0.224|0.374|0.738|
|930002|0.224|0.373|0.748|
|940002|0.226|0.369|0.746|
|950002|0.234|0.361|0.757|
|960002|0.233|0.371|0.770|
|970002|0.231|0.375|0.786|
|980002|0.240|0.386|0.781|
|990002|0.242|0.399|0.794|

## sortB
|Numbers|Average|1|2|3|4|5|6|7|8|9|10|
|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
|**Ascending**|
|25000 A|0.96|0.96|0.96|0.96|0.96|0.96|0.96|0.96|0.96|0.96|0.96|
|50000 A|3.87|3.87|3.87|3.87|3.87|3.87|3.87|3.87|3.87|3.87|
|**Descending**|
|25000 D|4.452|4.50|4.40|4.42|4.55|4.44|4.40|4.43|4.51|4.43|4.44|
|50000 D|17.927|17.74|18.14|17.74|17.78|17.86|18.84|17.52|18.06|17.80|17.79|
|**Random**|
|25000 R|3.658|3.70|3.65|3.61|3.57|3.59|3.70|3.68|3.72|3.74|3.62|
|50000 R|14.954|14.90|14.89|14.80|14.86|14.74|15.43|15.00|15.20|15.02|14.70|

**Further Timing**

|# Items|Ascending|Descending|Random|
|--:|:--:|:--:|:--:|
|2|0.000|0.000|0.000|
|1002|0.000|0.000|0.000|
|2002|0.000|0.021|0.020|
|3002|0.010|0.061|0.041|
|4002|0.020|0.113|0.084|
|5002|0.039|0.175|0.135|
|6002|0.050|0.259|0.202|
|7002|0.070|0.349|0.281|
|8002|0.100|0.451|0.365|
|9002|0.120|0.573|0.468|
|10002|0.150|0.720|0.588|
|11002|0.180|0.859|0.729|
|12002|0.220|1.008|0.859|
|13002|0.260|1.189|1.014|
|14002|0.300|1.403|1.180|
|15002|0.340|1.609|1.361|
|16002|0.390|1.827|1.521|
|17002|0.440|2.060|1.732|
|18002|0.500|2.331|1.950|
|19002|0.560|2.592|2.201|
|20002|0.620|2.857|2.444|
|21002|0.680|3.139|2.652|
|22002|0.746|3.460|2.945|
|23002|0.819|3.797|3.182|
|24002|0.890|4.101|3.493|
|25002|0.960|4.443|3.737|
|26002|1.040|4.804|4.111|
|27002|1.120|5.209|4.416|
|28002|1.210|5.548|4.757|
|29002|1.300|5.975|5.114|
|30002|1.390|6.403|5.435|
|31002|1.480|6.828|5.816|

|echo -e &quot;1 C\n2\n1 A\n3\n1 B\n4\n5&quot; \| ./sortB|
|:--|
|1 C<br>1 A<br>1 B<br>2<br>3<br>4<br>5<br>|
 
# Analysis
## sortA
|Numbers|Avg|
|:--:|:--:|
|**Ascending**|
|500000 A|0.127|
|1000000 A|0.262|
|**Descending**|
|500000 D|0.179|
|1000000 D|0.395|
|**Random**|
|500000 R|0.383|
|1000000 R|0.808|

The sort timing results suggested the implementation of a Bubble or Insertion Sort for `sortA`'s algorithm.  

In the best case scenario where all inputs are already sorted (ascending inputs) -  
`sortA` produced timings whose input of 1000000 numbers resulted in a timing of `0.262s`, which was twice that of half as many inputs (500000) at `0.127s`.  
This suggests that the sort algorithm was either a Bubble Sort or Insertion Sort, as both of these algorithms exhibit a best case time of O(n).

To determine whether this algorithm was a Bubble or Insertion sort, the command was run: `python3 -c "print('\n'.join(map(str,[1,*([3]*(1000000-2)),2])))" | time --format="%U" ./sortA > /dev/null`  
This returned a timing in the `0.2s` region which is closer to `O(n)` time than `O(n^2)`, hence it can be concluded that `sortA` is an Insertion Sort

<s>The stability sort passed, as the program output the correct order **CAB**</s> **BUT DIS WRONG, shell sorts are not stable by nature**


![](Snipaste_2019-01-22_16-05-48.png)

## sortB
|Numbers|Time (s)|
|:--:|:--:|
|**Ascending**|
|25000 A|0.96|
|50000 A|3.87|
|**Descending**|
|25000 D|4.452|
|50000 D|17.927|
|**Random**|
|25000 R|3.658|
|50000 R|14.954|

It is evident that `sortB` is neither an Insertion Sort nor a Bubble Sort, as a doubling of the number of items does not double proportionally. It is also not a selection sort, the timings do not increase exponentially

`sortB`'s sorting computation time graph displayed a non-linear trend, where the linear increase of the amount of items to sort increased either logarithmically or exponentially.

On assumption of a Shell Sort, upon dividing the timings by the number of items, a trend can be seen where the average and worst cases have values are the square of the best case timings.
This is consistent with the Shell Sort's complexity.

![](Snipaste_2019-01-24_15-51-44.png)

The stability sort passed, as the program output the correct order **CAB**

![](Snipaste_2019-01-22_16-04-54.png)

# Conclusion
`sortA` is an Insertion Sort.  
`sortB` is a Shell Sort.

EDIT: I dun guf.
`sortA` was a shell sort - unstable.  --> not enough input
`sortB` was a bubble sort - it has an early exit boi (best case)