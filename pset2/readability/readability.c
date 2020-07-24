#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

// define variables
int count_letters(string s);
int count_words(string s);
int count_sentences(string s);
float calculate_index(int letters, int words, int sentences);


int main(void)
{
    // prompt user for string
    string read = get_string("Text: ");
    // count letters
    int letters = count_letters(read);
    // count words
    int words = count_words(read);
    // count sentences
    int sentences = count_sentences(read);
    // calculate Coleman-Liau index
    float index = calculate_index(letters, words, sentences);
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %.0f\n", index);
    }
}


int count_letters(string s)
{
    int count = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (isalpha(s[i]))
        {
            count++;
        }
    }
    return count;
}


int count_words(string s)
{
    int count = 1;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (isspace(s[i]))
        {
            count++;
        }
    }
    return count;
}


int count_sentences(string s)
{
    int count = 0;
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (s[i] == '.' || s[i] == '?' || s[i] == '!')
        {
            count++;
        }
    }
    return count;
}


// calculate Coleman-Liau index
float calculate_index(int letters, int words, int sentences)
{
    // index = 0.0588 * L - 0.296 * S - 15.8
    float L = (float) letters / (float) words * 100;
    float S = (float) sentences / (float) words * 100;
    return 0.0588 * L - 0.296 * S - 15.8;
}
