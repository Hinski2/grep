#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "../structs/pair.h"
#include "../structs/vector.h"

// handlers
char* handle_args(int argc, char *argv[]); // return path or NULL
void handle_file(char *dir);    // takes ownership of the char*
void handle_dir(char *dir);     // takes ownership of the char*
void handle_line(char *line, size_t len, Pair_int (*search_fun)(char*, size_t, size_t), char *path, size_t line_no);

// search
void init_search();
Pair_int search_pattern_str(char *line, size_t len, size_t start); // if couldn't find returns {-1, -1}
Pair_int search_pattern_regex(char *line, size_t len, size_t start);

// printing
void print_line(char* line, size_t len, Vector_Pair_int* vec_occ, char *path, size_t line_no);

// utils
char* join_path(char *dir, char *name); // joins two char* into path and returns heap ptr to this new char*
char char_to_upper(char c);
void check_dir(char *dir);

#endif