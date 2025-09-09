#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "utils/utils.h"

// flags
bool LINE_NO = false;       // -n print line number
bool IGNORE_CASE = false;   // -i case insensitive              
bool COUNT = false;         // -c count lines
bool RECURSIVE = false;     // -r search recursively
bool USE_REGEX = false;     // -regex use regex for search 
bool DEBUG_INPUT = false;   // -d to print input

char *pattern = NULL;
size_t pattern_occ_cnt;

void debug_input(char *path) {

    // print flags
    printf("LINE_NO: %d\n", LINE_NO);
    printf("IGNORE_CASE: %d\n", IGNORE_CASE);
    printf("COUNT: %d\n", COUNT);
    printf("RECURSIVE: %d\n", RECURSIVE);
    printf("USE_REGEX: %d\n", USE_REGEX);

    // print chars
    printf("path: %s\n", path);
    if (pattern != NULL) {
        printf("PATTERN: %s\n", pattern);
    } else {
        printf("PATTERN: (null)\n");
    }
}

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("There is nothing to search for\n");
        exit(EXIT_SUCCESS);
    }

    char *path = handle_args(argc, argv);
    check_dir(path);
    if(DEBUG_INPUT)debug_input(path);

    if(RECURSIVE) {
        handle_dir(path);
    } else {
        handle_file(path);
    }

    if(COUNT) {
        printf("pattern occured: %d times\n", pattern_occ_cnt);
    }
}