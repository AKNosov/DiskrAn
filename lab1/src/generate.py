import random
import string

def generate_car_number():
    letter1 = random.choice(string.ascii_uppercase)
    number = ''.join(random.choices(string.digits, k=3))
    letter2 = random.choice(string.ascii_uppercase)
    letter3 = random.choice(string.ascii_uppercase)
    return f"{letter1} {number} {letter2}{letter3}"

def generate_value(length=64):
    value = ''.join(random.choices(string.ascii_lowercase + string.digits, k=random.randint(1, length)))
    return value.ljust(length, '\0') 

b = "\t"
c = "\0"
a = "\n"
def generate_key_value_pairs(num_pairs=30, filename='input.txt'):
    with open(filename, 'w') as file:
        for _ in range(num_pairs):
            key = generate_car_number()
            value = generate_value()
            file.write(f"{key}{b}{value.rstrip(c)}{a}")
generate_key_value_pairs(30)