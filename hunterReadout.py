# But why.

#!/bin/python3
import re

gameOver = False
messages = ["Rounds:", "Score:"]
while True:
    try:
        line = input("")
        if line == "> Game Over":
            gameOver = True
            print("\n--------- GAME OVER ---------")
        elif "GM>" in line or gameOver:
            if gameOver: print(messages.pop(), line)
            else: print(line)


    except:
        break

"""
function p(inputArr) {
  for (var i = 0; i < inputArr.length; i += 5) {
    console.log(inputArr.slice(i, i+5))
  }
}
"""