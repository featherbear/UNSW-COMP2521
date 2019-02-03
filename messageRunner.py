#!/bin/python3
import json

last = None
counter = 0

while True:
  try:
    line = input("")
    if line.startswith("  messages: "):
      data = json.loads(line[len("  messages: "):])
      i = 0
      while i < len(data):
        roundData = data[i:i+5]
        if last == roundData:
           counter += 1
        else:
           if last:
             if counter > 1:
               print(last,"x%d" % counter)
             else:
               print(last)
           counter = 1
           last = roundData
        i += 5

      if last:
       if counter > 1:
         print(last,"x%d" % counter)
       else:
         print(last)
    else:
      print(line)
  except: break
