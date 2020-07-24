#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

bool check_key(string key);
void encrypt_text(string plaintext, int plaintext_length, char *ciphertext, int *key_dif);
const int key_length = 26;
const string ALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


int main(int argc, string argv[])
{
    // accept only one command-line argument
    if (argc == 2)
    {
        string key = argv[1];
        // accept 26
        if (strlen(key) == key_length && check_key(key))
        {
            // prompt user for string
            string plaintext = get_string("plaintext: ");
            int plaintext_length = strlen(plaintext);
            char ciphertext[plaintext_length + 1];
            // array with integer distances from key to each letter
            int key_dif[key_length];
            for (int i = 0; i < key_length; i++)
            {
                key_dif[i] = toupper(key[i]) - ALPHA[i];
            }

            encrypt_text(plaintext, plaintext_length, ciphertext, key_dif);
            printf("ciphertext: %s\n", ciphertext);
        }
        else
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
}


void encrypt_text(string plaintext, int plaintext_length, char *ciphertext, int *key_dif)
{
    // loop over length of plaintext
    for (int i = 0; i < plaintext_length; i++)
    {
        // loop over alphabet
        for (int j = 0; j < key_length; j++)
        {
            // nonalpha chars copied through
            if (isalpha(plaintext[i]))
            {
                // key difference applied
                if (isupper(plaintext[i]) && plaintext[i] == ALPHA[j])
                {
                    ciphertext[i] = plaintext[i] + key_dif[j];
                }
                else if (islower(plaintext[i]) && plaintext[i] == tolower(ALPHA[j]))
                {
                    ciphertext[i] = plaintext[i] + key_dif[j];
                }
            }
            else
            {
                ciphertext[i] = plaintext[i];
            }
        }
    }
    // add null character to end of char array
    ciphertext[plaintext_length] = '\0';
}


bool check_key(string key)
{
    // accept only alpha characters for key
    for (int i = 0; i < key_length; i++)
    {
        if (!isalpha(key[i]))
        {
            return false;
            break;
        }
    }
    // check only one instance of each alpha character in key
    int count = 0;
    // loop through alphabet
    for (int j = 0; j < key_length; j++)
    {
        // loop through key
        for (int k = 0; k < key_length; k++)
        {
            if (isalpha(key[k]) && (ALPHA[j] == key[k] || tolower(ALPHA[j]) == key[k]))
            {
                count++;
                break;
            }
        }
    }
    if (count == key_length)
    {
        return true;
    }
    else
    {
        return false;
    }
}