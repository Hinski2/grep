#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "utils.h"

// flags
extern bool LINE_NO;       // -n print line number
extern bool IGNORE_CASE;   // -i case insensitive     
extern bool COUNT;         // -c count lines
extern bool RECURSIVE;     // -r search recursively
extern bool USE_REGEX;     // -regexp use regex for search

// path is a path to currently open file / dir that wi will use
extern char *path, *pattern;

Pair_int search_pattern_str(char *line, size_t len, size_t start) { // TODO change it on kmp
    size_t pattern_len = strlen(pattern);
    for(int i = start; i + pattern_len < len; i++) {
        bool ok = true;
        for(int j = 0; j < pattern_len && ok; j++) {
            if(line[i + j] != pattern[j]) ok = false;
        }

        if(ok) {
            return (Pair_int){i, i + pattern_len - 1};
        }
    }

    return (Pair_int){-1, -1};
}

void print_line(char *line, size_t len, Vector_Pair_int *vec_occ) {
    printf("\033[31m%s\033[0m ", path);
    printf("\033[32m%s\033[0m", line);
}

void handle_file(FILE *file) {
    Pair_int (*search_fun)(char*, size_t, size_t) = USE_REGEX ? search_pattern_regex : search_pattern_str;
    char *line = NULL;
    size_t len = 0;

    while(getline(&line, &len, file) != -1) {
        handle_line(line, len, search_fun);
    }

    free(line);
}

void handle_line(char *line, size_t len, Pair_int (*search_fun)(char*, size_t, size_t)) {
    size_t cursor = 0;
    Vector_Pair_int vec_occ; Vector_Pair_int_init(&vec_occ);
    Pair_int occ = {-1, -1};

    while((occ = search_fun(line, len, cursor)).fi != -1) {
        cursor = (int)occ.se + 1;
        Vector_Pair_int_push_back(&vec_occ, occ);
    }

    if(vec_occ.size) print_line(line, len, &vec_occ);

    // cleanup
    Vector_Pair_int_deinit(&vec_occ);
}

void handle_args(int argc, char *argv[]) {
    int i = 1;
    for(; i < argc; i++) {
        if(strcmp("-n", argv[i]) == 0) LINE_NO = true;
        else if(strcmp("-i", argv[i]) == 0) IGNORE_CASE = true;
        else if(strcmp("-c", argv[i]) == 0) COUNT = true;
        else if(strcmp("-r", argv[i]) == 0) RECURSIVE = true;
        else if(strcmp("-regexp", argv[i]) == 0) USE_REGEX = true;
        else if(strcmp("--", argv[i]) == 0) break;
        else if(!pattern) pattern = argv[i];
        else if(!path) path = argv[i];
        else {
            printf("too much arguments\n");
            exit(EXIT_FAILURE);
        }
    }

    for(i++; i < argc; i++) {
        if(!pattern) pattern = argv[i];
        else if(!path) path = argv[i];
        else {
            printf("too much arguments\n");
            exit(EXIT_FAILURE);
        }
    }
}

void check_dir() {
    if(RECURSIVE) {
        struct stat st;
        if(stat(path, &st) != 0) {
            printf("the path %s is innapropriate\n", path);
        }
    } else {
        if(access(path, F_OK) == -1){
            printf("the file %s doesn't exists\n", path);
        }
    }
}


// TODO
void handle_dir(char *dir) {};
void init_search() {};
Pair_int search_pattern_regex(char *line, size_t len, size_t start) {};