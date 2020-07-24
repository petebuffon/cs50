#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // prompt user for integer between 1 and 8
    int height;

    do
    {
        height = get_int("height: ");
    }
    while (height <= 0 || height > 8);

    // generate pyramid

    // row loop
    for (int i = 0; i < height; i++)
    {
        // first set of spaces
        for (int j = height - 2; j >= i; j--)
        {
            printf(" ");
        }
        // first set of hashes
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }
        // empty two spaces
        printf("  ");
        // second set of hashes
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }
        // row newline
        printf("\n");
    }
}
