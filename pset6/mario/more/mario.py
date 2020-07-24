"""Prints out a double half-pyramid of a specified height."""
from cs50 import get_int

# user input
while True:
    height = get_int("height: ")
    if height > 0 and height <= 8:
        break

# print out double half-pyramid
for i in range(1, height + 1):
    print(" " * (height - i),
          "#" * i,
          "  ",
          "#" * i,
          sep="")
