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

#for n in range(100, int(sys.argv[1]) + 1, 100):
n = 100
while True:
    print(str(n) + ",", file=sys.stderr, end=" ")

    nums = list(range(n))                            # Ascending
    if sys.argv[1] == "D": nums = nums[::-1]         # Descending
    elif sys.argv[1] == "R": shuffle(nums)             # Random
    string = "\n".join(map(str, [n] + nums)).encode()

    times = []
    for x in range(5):
        with subprocess.Popen(["/usr/bin/time", "-f%U", "./quicksort", "-pr", "-q"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE) as process:
            comm = process.communicate(string)
            times.append(float(comm[1].decode().strip()))

    avg = average(times)
    print(str(n) + ",", avg)
    print(avg, file=sys.stderr)

    if avg > 1.0: break
    if n == 500000: break

    if n < 10000: n += 100
    else: n += 2000