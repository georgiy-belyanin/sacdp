from colorama import Fore, Back, Style
import subprocess
import sys
import os

data_dir = 'data'
external_sat = 'picosat'
our_sat = './build/dpll'

for cnf in os.scandir(data_dir):
    r1 = subprocess.run([our_sat, cnf.path], stdout=subprocess.PIPE)
    r2 = subprocess.run([external_sat, cnf.path], stdout=subprocess.PIPE)
    if r1.stdout[0:7] != r2.stdout[0:7]:
        print(f"{cnf.path} is {Fore.RED}wrong{Style.RESET_ALL}")
        os.exit(-1)
    else:
        print(f"{cnf.path} is {Fore.GREEN}ok{Style.RESET_ALL}")
