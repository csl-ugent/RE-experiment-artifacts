// C/C++ headers
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Own headers
#include "structures.h"

static string_set_t* global_set;
static hashmap_t* op_map;

void setUpDataStructures() {
    // Map structure used for OP, contains 20 random int:int mappings
    // Command: ./insert_random_numbers.py '   hashmap_set(op_map, __RAND__, (void*)__RAND__);' --seed 443638
    op_map = hashmap_new(1024);
    hashmap_set(op_map, 8071, (void*)7153);
    hashmap_set(op_map, 9707, (void*)5586);
    hashmap_set(op_map, 2814, (void*)6454);
    hashmap_set(op_map, 4738, (void*)8546);
    hashmap_set(op_map, 191, (void*)8403);
    hashmap_set(op_map, 988, (void*)448);
    hashmap_set(op_map, 1747, (void*)3354);
    hashmap_set(op_map, 5913, (void*)6751);
    hashmap_set(op_map, 5222, (void*)8237);
    hashmap_set(op_map, 2930, (void*)8214);
    hashmap_set(op_map, 3360, (void*)4995);
    hashmap_set(op_map, 9365, (void*)7421);
    hashmap_set(op_map, 6816, (void*)1328);
    hashmap_set(op_map, 7732, (void*)2685);
    hashmap_set(op_map, 4813, (void*)5865);
    hashmap_set(op_map, 8865, (void*)6610);
    hashmap_set(op_map, 5030, (void*)4927);
    hashmap_set(op_map, 5799, (void*)9764);
    hashmap_set(op_map, 92, (void*)9134);
    hashmap_set(op_map, 4425, (void*)7920);

    // Set structure used in checking
    global_set = new_set();
    add_to_set(global_set, "bart");
    add_to_set(global_set, "bar");
    add_to_set(global_set, "apple");
    add_to_set(global_set, "applet");
    add_to_set(global_set, "tiger");
    add_to_set(global_set, "tigers");
}

// Structure word-word-word-word-...-word (12 words separated by -), where we have a repetition of 4 chunks of [word is in the set, word is not in the set but that same word with last character removed is in the set, word is not in the set]
// More words can be added after those 12, but are ignored
// Max length of 1 word: 10 characters. ASCII lowercase only.
// Words should be unique in the string, they can occur only once!
// For example, bart-barts-bert-bar-applets-berts-apple-bars-thomas-tiger-tigerss-rabbit
int get_next_word_ok(char** input, char* current_word) {
    memset(current_word, '\0', 11);
    for (int i = 0; i < 11; i++) {
        if (**input == '\0' || **input == '-') {
            if (i > 0)
                return 1;
            return 0; // empty word: not ok!
        }

        if (**input < 'a' || **input > 'z')
            return 0;

        current_word[i] = **input;
        (*input)++;
    }

    return 1;
}

// RNC possible solution: 76133-169499-262865-356231-449597

/*
 fused loop implementation by bjorn with the following structure
   INIT CHECK WITH SET
   INIT CHECK WITH RNC
 
   for (unsigned i = 0; i < 5; i++) {
       iteration of LOOP OF CHECK WITH RNC
       if (i==4) continue;
       iteration of LOOP OF CHECK WITH SET
   }
 */

bool checkKeyBjorn(char* input1, char* input2)
{
    // CHECK SET initialization
    string_set_t* words_seen_set = new_set();
    char current_word[11];
    
    
    // CHECK RNC initialization
    long seen_numbers[5];
    
    // Tokenize input2, return false if we don't get 5 tokens out of this
    const char* delim = "-";
    char* token = strtok(input2, delim);
    
    //FUSED LOOPS
    for (unsigned i = 0; i < 5; i++)
    {
        
        // CHECK RNC LOOP BODY
        // If we don't manage to get to 5 tokens, we're very disappointed
        if (!token)
            return false;
        
        // Parse as decimal number
        long num = strtol(token, NULL, 10);
        if (num == LONG_MAX || num == LONG_MIN)
            return false;
        
        // Check we haven't seen it before
        seen_numbers[i] = num;
        for (unsigned j = 0; j < i; j++)
        {
            if (seen_numbers[j] == num)
                return false;
        }
        
        num -= 10000;
        if (num <= 0)
            return false;
        if ((num % 14) != 11)
            return false;
        if ((num % 19) != 13)
            return false;
        if ((num % 13) != 2)
            return false;
        if ((num % 54) != 37)
            return false;
        
        // Get the next token
        token = strtok(NULL, delim);
        
        // CHECK SET LOOP BODY
        
        if (i==4) continue;
        
        for (int j = 0; j < 3; j++) {
            if (*input1 == '\0')
                return false;
            if (!get_next_word_ok(&input1, current_word))
                return false;
            if (*input1 == '-')
                input1++;
            
            if (is_in_set(words_seen_set, current_word))
                return false;
            add_to_set(words_seen_set, current_word);
            
            switch (j) {
                case 0: // First word: should be in set!
                    if (!is_in_set(global_set, current_word))
                        return false;
                    break;
                case 1: // Second word: should not be in set! But if we remove the last character, that should be in set!
                    if (is_in_set(global_set, current_word))
                        return false;
                    int l = strlen(current_word);
                    if (l < 2) {
                        // degenerate case
                        return false;
                    }
                    current_word[l-1] = '\0';
                    if (!is_in_set(global_set, current_word))
                        return false;
                    break;
                case 2: // Third word: should not be in set!
                    if (is_in_set(global_set, current_word))
                        return false;
                    break;
                default: // Should not happen
                    return false;
            }
        }
    }
    
    return true;
}

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    printf("Wrong number of arguments!\n");
    return -1;
  }

  setUpDataStructures();

  bool correct = checkKeyBjorn(argv[1], argv[2]);

  // Output result
  if (correct)
    printf("Correct Key!\n");
  else
    printf("Wrong Key!\n");
}
