#include <stdio.h>
#include <cs50.h>

int calculate_first_digit(long credit);
int calculate_second_digit(long credit);
int step_1(int digit, int pos, int sum);
int step_2(int digit, int pos, int sum);

int main(void)
{
    // prompt user for credit card number
    long credit = get_long("Number: ");

    int first_digit = calculate_first_digit(credit);
    int second_digit = calculate_second_digit(credit);

    int sum = 0;
    int pos = 0;
    // get individual digits from cc number
    while (credit > 0)
    {
        int digit = credit % 10;
        pos++;
        sum = step_1(digit, pos, sum);
        sum = step_2(digit, pos, sum);
        if (digit == 0)
        {
            credit = credit / 10;
        }
        else
        {
            credit = (credit - digit) / 10;
        }

    }

    // algorithm result and cc numbers must be 15 digits or higher
    if (sum % 10 == 0 && pos > 14)
    {
        // AMEX have 15 digits and start with either 34 or 37
        if (pos == 15 && first_digit == 3 && (second_digit == 4 || second_digit == 7))
        {
            printf("AMEX\n");
        }
        // MASTERCARD have 16 digits and star with 51, 52, 53, 54, or 55
        else if (pos == 16 && first_digit == 5 && (second_digit == 1 || second_digit == 2 || second_digit == 3 || second_digit == 4 || second_digit == 5))
        {
            printf("MASTERCARD\n");
        }
        // VISA have either 13 or 16 digits and start with 4
        else if ((pos == 13 || pos == 16) && first_digit == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}


// algorithm step 1
int step_1(int digit, int pos, int sum)
{
    // every other digit starting with the second-to-last digit
    if (pos % 2 == 0)
    {
        int m_digit = digit * 2;
        // add digits of resulting products over 10
        if (m_digit > 9)
        {
            sum = sum + m_digit % 10 + 1;
        }
        else
        {
            sum = sum + m_digit;
        }
    }
    return sum;
}


// algorithm step 2
int step_2(int digit, int pos, int sum)
{
    // every other digit starting with last digit
    if (pos % 2 != 0)
    {
        sum = sum + digit;
    }
    return sum;
}


// calculate first digit of cc number
int calculate_first_digit(long credit)
{
    while (credit > 10)
    {
        credit = credit / 10;
    }
    return credit;
}


// calculate second digit of cc number
int calculate_second_digit(long credit)
{
    while (credit > 100)
    {
        credit = credit / 10;
    }
    credit = credit % 10;
    return credit;
}