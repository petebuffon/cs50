"""determines whether a provided credit card number is valid according to Luhn’s algorithm."""
from cs50 import get_string

credit = get_string("Number: ")

# algorithm step one
luhn1 = list(credit[::-1][1::2])
luhn1 = list(map(int, luhn1))
l = 0
for i in luhn1:
    if i * 2 > 9:
        l += i * 2 % 10 + 1
    else:
        l += i * 2

# algorithm step two
luhn2 = credit[::-2]
luhn2 = list(map(int, luhn2))
l += sum(luhn2)

n = len(credit)
b = credit[0:2]
# algorithm result and cc numbers must be 15 digits or higher
if l % 10 == 0 and n > 14:
    # AMEX have 15 digits and start with either 34 or 37
    if n == 15 and (b == '37' or b == '37'):
        print('AMEX')
    # MASTERCARD have 16 digits and star with 51, 52, 53, 54, or 55
    elif n == 16 and (b == '51' or b == '52' or b == '53' or b == '54' or b == '55'):
        print('MASTERCARD')
    elif (n == 13 or n == 16) and b[0] == '4':
        print('VISA')
    else:
        pass
else:
    print('INVALID')
