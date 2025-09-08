#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "utils/utils.h"

// flags
bool LINE_NO = false;       // -n print line number
bool IGNORE_CASE = false;   // -i case insensitive              //TODO
bool COUNT = false;         // -c count lines
bool RECURSIVE = false;     // -r search recursively
bool USE_REGEX = false;     // -regexp use regex for search     //TODO

char *pattern = NULL;
size_t pattern_occ_cnt;

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("There is nothing to search for\n");
        exit(EXIT_SUCCESS);
    }

    char *path = handle_args(argc, argv);
    check_dir(path);

    if(RECURSIVE) {
        handle_dir(path);
    } else {
        handle_file(path);
    }

    if(COUNT) {
        printf("pattern occured: %d times\n", pattern_occ_cnt);
    }
}