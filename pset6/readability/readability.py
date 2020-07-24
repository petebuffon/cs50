"""Computes the approximate grade level needed to comprehend some text."""
from cs50 import get_string

# prompt user for string
read = get_string("Text: ")


def main():
    # count letters
    letters = count_letters(read)
    # count words
    words = count_words(read)
    # count sentences
    sentences = count_sentences(read)
    # calculate Coleman-Liau index
    index = calculate_index(letters, words, sentences)
    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {int(round(index, 0))}")


def count_letters(s):
    """Count letters"""
    cnt = 0
    for chr in s:
        if chr.isalpha():
            cnt += 1
    return cnt


def count_words(s):
    """Count words"""
    cnt = 1
    for chr in s:
        if chr == " ":
            cnt += 1
    return cnt


def count_sentences(s):
    """Count sentences"""
    cnt = 0
    for chr in s:
        if chr is "." or chr is "?" or chr is "!":
            cnt += 1
    return cnt


def calculate_index(letters, words, sentences):
    """Calculate Coleman-Liau index"""
    # index = 0.0588 * L - 0.296 * S - 15.8
    L = letters / words * 100
    S = sentences / words * 100
    return 0.0588 * L - 0.296 * S - 15.8


main()
