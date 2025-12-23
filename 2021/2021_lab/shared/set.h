#ifndef SET_H
#define SET_H

typedef struct string_set string_set_t;

string_set_t* new_set();
void delete_set(string_set_t*);
void remove_from_set(string_set_t*, char*);
void add_to_set(string_set_t*, char*);
int is_in_set(string_set_t*, char*);


#endif /* SET_H */

