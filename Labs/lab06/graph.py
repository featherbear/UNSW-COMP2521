import sys

import subprocess
from random import shuffle

def average(items):
  sum = 0
  for item in items:
    sum += item
  return round(sum / len(items),3)


res = None
for n in range(2, int(sys.argv[1]), 1000):
  print(n, file=sys.stderr)
  if sys.argv[2] == "D": nums = range(n, -1, -1)   # Descending
  else: nums = range(n)                            # Ascending
  
  nums = list(nums)
  if sys.argv[2] == "R": shuffle(nums)             # Random
  string = "\n".join(map(str,nums)).encode()

  times = []
  for x in range(10):
    with subprocess.Popen(["/usr/bin/time", "--format=\"%U\"", "./sortB"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE) as process:
      times.append(float(process.communicate(string)[1][1:-2]))

  print(str(n) + ",", average(times))


