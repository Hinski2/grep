#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "../structs/pair.h"
#include "../structs/vector.h"

char* handle_args(int argc, char *argv[]); // return path or NULL
void check_dir(char *dir);
void handle_file(char *dir);    // takes ownership of the char*
void handle_dir(char *dir);     // takes ownership of the char*

void handle_line(char *line, size_t len, Pair_int (*search_fun)(char*, size_t, size_t), char *path, size_t line_no);
// joins two char* into path and returns heap ptr to this new char*
char* join_path(char *dir, char *name);

void init_search(); // TODO when we will have kmp and regex
// if couldn't find returns {-1, -1}
Pair_int search_pattern_str(char *line, size_t len, size_t start);
Pair_int search_pattern_regex(char *line, size_t len, size_t start);

void print_line(char* line, size_t len, Vector_Pair_int* vec_occ, char *path, size_t line_no);

#endif