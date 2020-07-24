#include <cs50.h>
#include <stdio.h>

void selection_sort(void);

int array[6] = {5,2,1,3,6,4};
int array_len = 6;

int main(void)
{
    selection_sort();
    for (int i = 0; i < array_len; i++)
        printf("%i\n", array[i]);
}


void selection_sort(void)
{
    int sml_num;
    int sml_pos;
    for (int i = 0; i < array_len; i++)
    {
        sml_pos = i;
        sml_num = array[i];
        for (int j = i + 1; j < array_len; j++)
        {
            if (array[j] < sml_num)
            {
                sml_num = array[j];
                sml_pos = j;
            }
        }
        array[sml_pos] = array[i];
        array[i] = sml_num;

    }
}