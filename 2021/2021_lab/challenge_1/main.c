// C/C++ headers
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Own headers
#include "structures.h"

static hashmap_t* op_map;

void setUpDataStructures() {
    // Map structure used for OP, contains 20 random int:int mappings
    // Command: ./insert_random_numbers.py '   hashmap_set(op_map, __RAND__, (void*)__RAND__);' --seed 443637
    op_map = hashmap_new(1024);
    hashmap_set(op_map, 2220, (void*)7202);
    hashmap_set(op_map, 7765, (void*)8208);
    hashmap_set(op_map, 1000, (void*)2985);
    hashmap_set(op_map, 748, (void*)8707);
    hashmap_set(op_map, 6201, (void*)6566);
    hashmap_set(op_map, 5351, (void*)3870);
    hashmap_set(op_map, 8913, (void*)26);
    hashmap_set(op_map, 7025, (void*)1283);
    hashmap_set(op_map, 4846, (void*)8071);
    hashmap_set(op_map, 2736, (void*)1636);
    hashmap_set(op_map, 3022, (void*)7296);
    hashmap_set(op_map, 4645, (void*)6858);
    hashmap_set(op_map, 7787, (void*)6756);
    hashmap_set(op_map, 8520, (void*)2070);
    hashmap_set(op_map, 3916, (void*)5008);
    hashmap_set(op_map, 2991, (void*)8176);
    hashmap_set(op_map, 8699, (void*)9852);
    hashmap_set(op_map, 8378, (void*)16);
    hashmap_set(op_map, 6598, (void*)6168);
    hashmap_set(op_map, 2313, (void*)9042);
}

bool mergedCheck(char* input, unsigned part_index)
{
    if (part_index == 0)
    {
        // Check key layout: XXX_YYY, where both halves are of variable length
        // These numbers serve as input to the function f(X,Y) = 2 X^3 Y^3 + 400 X^3 Y^2 + 37 X^2 Y^3 - 10081 X^2 Y^2 + 2 X Y + 400 X + 37 Y - 10081
        // which equals (2XY + 400X + 37Y - 10081)*(X^2*Y^2 +1)
        // The key is correct when the function evaluates to 0
        // Example solution (X = -8759, Y = -201)

        const char* score = strchr(input, '_');
        if (!score)
            return false;

        // Read X, discard invalid arguments
        long x = strtol(input, NULL, 10);
        if ((x == LONG_MAX) || (x == LONG_MIN))
            return false;

        // Read Y, discard invalid arguments
        long y = strtol(score+1, NULL, 10);
        if ((y == LONG_MAX) || (y == LONG_MIN))
            return false;

        // Evaluate function
        long fun = 2 * x * x * x * y * y * y
            + 400 * x * x * x * y * y
            + 37 * x * x * y * y * y;

        long fun2 = - 10081 * x * x * y * y
            + 2 * x * y
            + 400 * x
            + 37 * y
            - 10081;

        if (fun + fun2 != 0)
            return false;
    }
    else
    {
        // Structure WWWW-XXXX-YYYY-ZZZZ, when converted to binary need to XOR to 0x1830 (=6192)
        // Every part needs to consist of exactly 4 decimals
        // (Counting from 0) the odd parts of the sequence need to be > than 250 decimal, the eveb ones < 250 decimal and should be != 0
        // Example of a correct key: 0001-6347-0001-0251
        char current_int[5];
        current_int[4] = '\0';
        int current_xor = 0;

        for (int i = 0; i < 4; i++) {
            // 4-digit integer
            for (int j = 0; j < 4; j++) {
                if (*input < '0' || *input > '9')
                    return false;
                current_int[j] = *input;
                input++;
            }
            // Separator or 0-terminator
            if (i < 3) {
                if (*input != '-')
                    return false;
            } else {
                if (*input != '\0')
                    return false;
            }
            input++;

            int parsed_int = atoi(current_int);

            if(i % 2 != 0) {
                if (parsed_int <= 250)
                    return false;
            } else {
                if (parsed_int >= 250)
                    return false;
                if (parsed_int == 0)
                    return false;
            }

            current_xor ^= parsed_int;
        }

        if (current_xor != 0x1830)
            return false;
    }
    return true;
}

bool checkKey(char* part1, char* part2)
{
  bool correct = true;
  correct &= mergedCheck(part1, 0);
  correct &= mergedCheck(part2, 1);

  return correct;
}

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    printf("Wrong number of arguments!\n");
    return -1;
  }

  setUpDataStructures();

  bool correct = checkKey(argv[1], argv[2]);

  // Output result
  if (correct)
    printf("Correct Key!\n");
  else
    printf("Wrong Key!\n");
}
