#!/usr/bin/python3
import argparse
import itertools
import random
import string

if __name__ == '__main__':
    # Parsing the arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('template', help='The template string to fill in.')
    parser.add_argument('-s', '--seed', type=int, default=443637, help='The seed for the RNG.')
    parser.add_argument('-l', '--length', default=8, help='The length of the random string to generate.')
    parser.add_argument('-n', '--number', default=20, help='The number of instances to generate.')
    parser.add_argument('-t', '--token', default='__RAND__', help='The token to replace with a random number.')
    parser.add_argument('-a', '--alphabet', default='', nargs='+', help='The tokens to use as a basis for the random strings.')
    args = parser.parse_args()

    random.seed(args.seed)
    template_parts = args.template.split(args.token)

    for _ in range(args.number):
        random_strings = []
        for _ in range(len(template_parts) -1):
            if (args.alphabet):
                # Choose a word to adapt, by either removing a character, or adding a random one
                baseWord = random.choice(args.alphabet)
                if random.choice([True, False]):
                    newWord = baseWord[:-1]
                else:
                    newWord = baseWord + random.choice(string.ascii_lowercase + string.digits)
            else:
                newWord = ''.join(random.choices(string.ascii_lowercase + string.digits, k=args.length))

            random_strings.append(newWord)

        filled_parts = [val for pair in itertools.zip_longest(template_parts, random_strings, fillvalue='') for val in pair]
        out_string = ''.join(filled_parts)
        print(out_string)
