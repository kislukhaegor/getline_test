from random import choice, randint
from string import ascii_letters
import argparse
import sys

def random_string(size):
    return ''.join(choice(ascii_letters) for _ in range(size -1 )) + '\n'

def generate(params):
    with open(params.filename, 'w') as f:
        size = 0
        while (size < params.size * 1024):
            len = randint(400, 800)
            f.write(random_string(len))
            size += len


def parse_args(args):
    parser = argparse.ArgumentParser(description='This is a simple file generator')
    parser.add_argument(
        '-f',
        '--filename',
        required=True,
        action='store',
        help='output filename')

    parser.add_argument(
        '-s',
        '--size',
        required=True,
        type=int,
        action="store",
        help='size of output file in kb')
    return parser.parse_args(args)

if __name__ == '__main__':
    params = parse_args(sys.argv[1:])
    generate(params)