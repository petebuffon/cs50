"""imports data from a CSV spreadsheet."""
from sys import argv, exit
import cs50
import csv

db = cs50.SQL("sqlite:///students.db")

# only accept 2 command-line arguments
if len(argv) != 2:
    print("Usage: python import.py csv")
    exit(1)

# open characters.csv
with open(argv[1], "r") as file:

    # create DictReader
    reader = csv.DictReader(file)

    # insert values into sql database
    for row in reader:
        # parse first, middle, and last names
        name = row["name"].split(" ")
        first = name[0]
        # set middle name to None if no middle name present
        if len(name) == 2:
            middle = None
            last = name[1]
        elif len(name) == 3:
            middle = name[1]
            last = name[2]
        house = row["house"]
        birth = row["birth"]
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
            first, middle, last, house, birth)
