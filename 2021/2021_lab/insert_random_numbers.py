#!/usr/bin/python3
import argparse
import itertools
import random

if __name__ == '__main__':
    # Parsing the arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('template', help='The template string to fill in.')
    parser.add_argument('-s', '--seed', type=int, default=443637, help='The seed for the RNG.')
    parser.add_argument('-b', '--begin', default=0, help='The begin of the range.')
    parser.add_argument('-e', '--end', default=9999, help='The end of the range.')
    parser.add_argument('-n', '--number', default=20, help='The number of instances to generate.')
    parser.add_argument('-t', '--token', default='__RAND__', help='The token to replace with a random number.')
    args = parser.parse_args()

    random.seed(args.seed)
    template_parts = args.template.split(args.token)

    for _ in range(args.number):
        random_numbers = []
        for _ in range(len(template_parts) -1):
            random_numbers.append(str(random.randint(args.begin, args.end)))

        filled_parts = [val for pair in itertools.zip_longest(template_parts, random_numbers, fillvalue='') for val in pair]
        out_string = ''.join(filled_parts)
        print(out_string)
