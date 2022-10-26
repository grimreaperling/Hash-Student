import json
import os
import random
import names
import string
from unique_names_generator import get_random_name
from unique_names_generator.data import ADJECTIVES, NAMES

file = open('students.txt', 'w+')
test = open('test.txt', 'w+')
letters = string.digits

for i in range(1200):
    string = ''.join(random.choice(letters) for i in range(10))
    string += ' '
    string += names.get_first_name()
    string += ' '
    nickname = get_random_name(separator="-", style="lowercase")
    string += nickname
    string += ' '
    string += "CS" + str(random.randint(100, 200))
    for i in range(3):
        string += ' ' + str(random.randint(60, 100))
    string += '\n'
    test.write(nickname + '\n')
    file.write(string)
    
    

