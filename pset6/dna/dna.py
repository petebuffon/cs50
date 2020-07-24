"""Identifies a person based on their DNA"""
from sys import argv, exit
import csv


def main():
    if len(argv) != 3:
        print("Usage: python dna.py database sequence")
        exit(1)

    with open(argv[1], "r") as csvfile:
        reader = csv.reader(csvfile)
        table = list(reader)

    with open(argv[2], "r") as file:
        sequence = file.read()

    STR_cnt = []
    STR_list = table[0][1:]
    for STR in STR_list:
        STR_cnt.append(count_STR(sequence, STR))

    person_list = table[1:]
    for person in person_list:
        if person[1:] == STR_cnt:
            print(person[0])
            exit(0)
    print("No match")


def count_STR(sequence, STR):
    """Counts highest number of a STR"""
    n = len(sequence)
    cnt_list = []
    # loop through each sequence position
    for i in range(n):
        # deal only with substring
        substring = sequence[i:]
        cnt = 0
        # check for STR and increment cnt if found
        for j in range(0, len(substring), len(STR)):
            d = substring[j:j+len(STR)]
            if d == STR:
                cnt += 1
            else:
                break
        cnt_list.append(cnt)
    return str(max(cnt_list))


main()
