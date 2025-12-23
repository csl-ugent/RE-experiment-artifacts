// C/C++ headers
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Own headers
#include "structures.h"

static graph_t* global_graph;
static node_t* global_nodes[10];
static string_set_t* op_set;

void setUpDataStructures()
{
    // Graph structure used in checking
    global_graph = GraphNew();
    global_nodes[0] = GraphAddNode(global_graph, "amsterdam");
    global_nodes[1] = GraphAddNode(global_graph, "brussels"),
    global_nodes[2] = GraphAddNode(global_graph, "cuzco"),
    global_nodes[3] = GraphAddNode(global_graph, "darwin"),
    global_nodes[4] = GraphAddNode(global_graph, "essen"),
    global_nodes[5] = GraphAddNode(global_graph, "freetown"),
    global_nodes[6] = GraphAddNode(global_graph, "guangzhou"),
    global_nodes[7] = GraphAddNode(global_graph, "houston"),
    global_nodes[8] = GraphAddNode(global_graph, "isfahan"),
    global_nodes[9] = GraphAddNode(global_graph, "jakarta");

    // Set structure used for OP, contains some more city names
    op_set = new_set();
    add_to_set(op_set, "palermo");
    add_to_set(op_set, "kingston");
    add_to_set(op_set, "moscow");
    add_to_set(op_set, "omsk");
    add_to_set(op_set, "reykjavik");
    add_to_set(op_set, "lima");
    add_to_set(op_set, "valladolid");
    add_to_set(op_set, "antioch");
    add_to_set(op_set, "dublin");
    add_to_set(op_set, "nanjing");
    add_to_set(op_set, "akra");
    add_to_set(op_set, "arrakis");
    add_to_set(op_set, "dubai");
}

#define EDGE_TRIPLET_LEN 4

// GRAPH possible solution: 018-028-038-048-058_04-102400
// RNC possible solution: 76133-169499-262865-356231-449597

/* fused loop implementation by bert with the following structure
   INIT CHECK WITH GRAPH
   INIT CHECK WITH RNC
 
   for (unsigned i = 0; i < 5; i++) {
       iteration of LOOP OF CHECK WITH RNC
       iteration of LOOP OF CHECK WITH GRAPH
   }
   FINI CHECK WITH GRAPH
 */
bool checkKey(char* part1, char* part2)
{

    // CHECK GRAPH initialization
    // Check key layout: XXX-XXX-XXX-XXX-XXX_XX-XXXXX
    // Two halves: XXX-XXX-XXX-XXX-XXX and XX-XXXXX
    // The first half of the key encodes the edges to add in the graph. Every triplet is (start node, end node, weight)
    // The second half of the key encodes the input (before -) and output (after -) for the algorithm
    // The algorithm calculates the how many routes one can take, starting from node A, and taking B steps.
    // The input is the number of the node to start from, and the number of steps
    // The output is then the number of possible routes
    if ((part1[EDGE_TRIPLET_LEN -1] != '-')
            || (part1[EDGE_TRIPLET_LEN*2 -1] != '-')
            || (part1[EDGE_TRIPLET_LEN*3 -1] != '-')
            || (part1[EDGE_TRIPLET_LEN*4 -1] != '-')
            || (part1[EDGE_TRIPLET_LEN*5 -1] != '_')
            || (part1[EDGE_TRIPLET_LEN*5 +2] != '-'))
        return false;

    // CHECK RNC initialization
    long seen_numbers[5];

    // Tokenize input, return false if we don't get 5 tokens out of this
    const char* delim = "-";
    char* token = strtok(part2, delim);
    char* token_obf;

    //FUSED LOOPS
    unsigned i;
    for (i = 0; i < 5; i++)
    {
        // CHECK RNC LOOP BODY
        // If we don't manage to get to 5 tokens, we're very disappointed
        if (!token)
            return false;

        // Parse as decimal number
        long num = strtol(token, NULL, 10);
        if (num == LONG_MAX || num == LONG_MIN)
            return false;

        // we will reuse the current token to encode the predicate
        token_obf = token;

        // hoisted predicate = false
        add_to_set(op_set, token_obf);

        // MANUPULATE DATA STRUCTURE WITHOUT CHANGING PREDICATE
        // goal is to add something  different from the token_obf to the set_obf
        int l = strlen(token_obf);
        char * token_dummy = malloc(l+1);
        strcpy(token_dummy,token_obf);
        if (l >= 2) {
            token_dummy[l-1] = '\0';
            add_to_set(op_set, token_dummy);
        }

        // Check we haven't seen it before
        seen_numbers[i] = num;
        for (unsigned j = 0; j < i; j++)
        {
            if (seen_numbers[j] == num)
                return false;
        }

        goto label_joined;
label1:

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

        // CHECK GRAPH LOOP BODY

        // Decode and add every edge
        // Get chars that encode every edge
        char a = part1[EDGE_TRIPLET_LEN * i +0];
        char b = part1[EDGE_TRIPLET_LEN * i +1];
        char c = part1[EDGE_TRIPLET_LEN * i +2];

        // hoisted predicate = true
        remove_from_set(op_set, token_obf);

        if (a < '0' || '9' < a)
            return false;
        if (b < '0' || '9' < b)
            return false;
        if (c < '0' || '9' < c)
            return false;
        unsigned start = a - '0';
        unsigned end = b - '0';
        unsigned weight = c - '0';
        goto label_joined;
label2:

        GraphAddEdge(global_graph, global_nodes[start], global_nodes[end], weight);
    }

    // CHECK GRAPH finalization

    // Decode inputs: starting node and number of steps
    const char* second_half = &part1[EDGE_TRIPLET_LEN *5];
    char a = second_half[0];
    char b = second_half[1];
    if (a < '0' || '9' < a)
        return false;
    if (b < '0' || '9' < b)
        return false;
    unsigned start = a - '0';
    unsigned nr_of_steps = b - '0';

    // Decode output: number of possible routes
    unsigned long int number_of_possible_routes = strtoul(second_half +3, NULL, 10);

    // Exclude lame results
    if (number_of_possible_routes == ULONG_MAX || number_of_possible_routes < 9999)
        return false;

    // Feed to algorithm and compare
    unsigned ret = GraphCalculateNumberOfRoutesFromNode(global_nodes[start], nr_of_steps);
    if (ret != number_of_possible_routes)
        return false;

    return true;


label_joined:
    {
      // insert code that changes data structure without changing predicate
      // insert some dead code that survives compilation
      char* current_word = &part1[EDGE_TRIPLET_LEN * i +0];
      if (current_word[0] && strcmp(current_word,token_obf))
         add_to_set(op_set,current_word);

      // if (predicate)
      if (!is_in_set(op_set, token_obf))
        goto label2;
      else
        goto label1;
    }
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
