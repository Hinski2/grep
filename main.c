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
bool USE_REGEX = false;     // -regexp use regex for search

char *path = NULL, *pattern = NULL;

int main(int argc, char *argv[]) {
    // printf("chuj kurwa chuj debuuuuuuug"); //!!!!!!!!!!!

    if(argc == 1) {
        printf("There is nothing to search for\n");
        exit(EXIT_SUCCESS);
    }


    handle_args(argc, argv);
    check_dir();

    if(RECURSIVE) {
        handle_dir(path);
    } else {
        FILE *file = path ? fopen(path, "r") : stdin;
        if(file == NULL) {
            printf("errror occured when opening file: %s", path);
            exit(EXIT_FAILURE);
        }

        handle_file(file);
    }
}