// C/C++ headers
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Own headers
#include "structures.h"

// A 'set' is really a sorted (on the element_value key) linked list, terminated by an element with a NULL next and element_value
struct string_set {
    string_set_t* next;
    char* element_value;
};

string_set_t* new_set() {
    string_set_t* s = (string_set_t*) malloc(sizeof(string_set_t));
    s->next = 0;
    s->element_value = 0;
    return s;
}

void delete_set(string_set_t* s) {
    // Let's ignore the linked list aspect for now ;)
    free(s);
}

void remove_from_set(string_set_t* s, char* word) {
    while(true) {
        if (!s->element_value) {
            // We are at the end, the word is not present
            return;
        }
        int cmp = strcmp(word, s->element_value);

        if (cmp == 0) {
            free(s->element_value);

            string_set_t* tofree = s->next;
            s->element_value = tofree->element_value;
            s->next = tofree->next;
            free(tofree);
            return;
        }

        s = s->next;
    }
}

// a b c f g
// insert e:
// e > a -> next
// e > b -> next
// e > c -> next
// e < f -> insert e here -> put f in a new, next element, overwrite current element with e

void add_to_set(string_set_t* s, char* word) {
    while(true) {
        if (!s->element_value) {
            // We are at the end: insert here!
            s->element_value = strdup(word);
            s->next = new_set();
            return;
        }
        int cmp = strcmp(word, s->element_value);

        if (cmp == 0) {
            // Already in the set!
            return;
        }
        if (cmp < 0) {
            // You want to insert the new element at the spot where it is smaller than the next element. That means it is larger than all previous elements, and smaller than the next one
            // And that is then where you insert it.
            string_set_t* new_element = new_set();
            new_element->next = s->next; // We insert the new element right behind the current list element s
            new_element->element_value = s->element_value;
            s->element_value = strdup(word);
            s->next = new_element; // new_element is > s
            return;
        }
        s = s->next;
    }
}

int is_in_set(string_set_t* s, char* word) {
    while(true) {
        if (!s->element_value)
            return 0;
        int cmp = strcmp(word, s->element_value);
        if (cmp == 0)
            return 1;
        if (cmp < 0) // ordered list, we keep looking as long as the word is larger than the current list_element. If the word is smaller than the current list_element, we are too far, and did not find it
            return 0;
        s = s->next;
    }
    return 0; // Should not happen...
}

