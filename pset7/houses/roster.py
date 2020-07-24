"""Prints a list of students for a given house in alphabetical order."""
from sys import argv, exit
import cs50

# only accept 2 command-line arguments
if len(argv) != 2:
    print("Usage: python roster.py house")
    exit(1)

# open database
db = cs50.SQL("sqlite:///students.db")

# query sql database for first, middle, and last name, and birth year by house
query = db.execute("SELECT first, middle, last, birth FROM students WHERE house is %s ORDER BY last, first", (argv[1],))

for student in query:
    # omit middle name if None
    if student["middle"] == None:
        print(f"{student['first']} {student['last']}, born {student['birth']}")
    else:
        print(f"{student['first']} {student['middle']} {student['last']}, born {student['birth']}")
