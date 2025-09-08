#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include "utils.h"

// flags
extern bool LINE_NO;       // -n print line number
extern bool IGNORE_CASE;   // -i case insensitive     
extern bool COUNT;         // -c count lines
extern bool RECURSIVE;     // -r search recursively
extern bool USE_REGEX;     // -regexp use regex for search

// path is a path to currently open file / dir that wi will use
extern char *pattern;
extern size_t pattern_occ_cnt;

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

void print_line(char *line, size_t len, Vector_Pair_int *vec_occ, char *path, size_t line_no) {
    if(COUNT) {
        pattern_occ_cnt += vec_occ -> size;
        return;
    }

    printf("\033[36m%s\033[0m", path);
    if(LINE_NO) {
        printf("\033[34m:%d\033[0m ", line_no);
    }
    printf(" ");

    Vector_Pair_int_reverse(vec_occ);
    Pair_int occ = Vector_Pair_int_pop_back(vec_occ);

    for(int i = 0; i < len; i++) {
        if(occ.fi <= i && i <= occ.se) {
            printf("\033[31m%c\033[0m", line[i]);
        } else {
            printf("%c", line[i]);
        }

        if(i >= occ.se && vec_occ -> size)
            occ = Vector_Pair_int_pop_back(vec_occ);
    }
}

void handle_file(char* dir) {
    FILE *file = dir ? fopen(dir, "r") : stdin;
    if(!file) {
        printf("couldn't open the file %s", dir);
        exit(EXIT_FAILURE);
    }

    Pair_int (*search_fun)(char*, size_t, size_t) = USE_REGEX ? search_pattern_regex : search_pattern_str;
    char *line = NULL;
    size_t cap = 0, line_no = 1;
    ssize_t nread;

    while((nread = getline(&line, &cap, file)) != -1) {
        handle_line(line, nread, search_fun, dir, line_no);
        line_no++;
    }

    free(line);
    if(dir) {
        fclose(file);
        free(dir);
    }
}

void handle_line(char *line, size_t len, Pair_int (*search_fun)(char*, size_t, size_t), char *path, size_t line_no) {
    size_t cursor = 0;
    Vector_Pair_int vec_occ; Vector_Pair_int_init(&vec_occ);
    Pair_int occ = {-1, -1};

    while((occ = search_fun(line, len, cursor)).fi != -1) {
        cursor = (int)occ.se + 1;
        Vector_Pair_int_push_back(&vec_occ, occ);
    }

    if(vec_occ.size) print_line(line, len, &vec_occ, path, line_no);

    // cleanup
    Vector_Pair_int_deinit(&vec_occ);
}

char* handle_args(int argc, char *argv[]) {
    int i = 1;
    char *path = NULL;
    for(; i < argc; i++) {
        if(strcmp("-n", argv[i]) == 0) LINE_NO = true;
        else if(strcmp("-i", argv[i]) == 0) IGNORE_CASE = true;
        else if(strcmp("-c", argv[i]) == 0) COUNT = true;
        else if(strcmp("-r", argv[i]) == 0) RECURSIVE = true;
        else if(strcmp("-regexp", argv[i]) == 0) USE_REGEX = true;
        else if(strcmp("--", argv[i]) == 0) break;
        else if(!pattern) pattern = argv[i];
        else if(!path) path = strdup(argv[i]);
        else {
            printf("too much arguments\n");
            exit(EXIT_FAILURE);
        }
    }

    for(i++; i < argc; i++) {
        if(!pattern) pattern = argv[i];
        else if(!path) path = strdup(argv[i]);
        else {
            printf("too much arguments\n");
            exit(EXIT_FAILURE);
        }
    }

    return path;
}

void check_dir(char *path) {
    if(!path) return;

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

char* join_path(char *dir, char *name) {
    size_t d_len = strlen(dir), n_len = strlen(name);
    size_t len = d_len + n_len + 2;
    char *file_name = malloc(len);
    if(!file_name) {
        printf("couldn't allocate space\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    for(; i < d_len; i++)
        file_name[i] = dir[i];

    file_name[i] = '/';
    for(i++; i < len; i++) 
        file_name[i] = name[i - d_len - 1];
    file_name[i] = '\0';

    return file_name;
}


void handle_dir(char *path) {
    DIR *dir = opendir(path);
    if(!dir) {
        printf("couldn't open the dir %s\n", path);
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    struct stat st;
    while((entry = readdir(dir)) != NULL) {
        if(strcmp(entry -> d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0) continue;
        char *file_name = join_path(path, entry -> d_name);

        if(stat(file_name, &st) == 0) {
            if(S_ISREG(st.st_mode)) handle_file(file_name);
            else handle_dir(file_name);
        } else {
            free(file_name);
        }
    }
    closedir(dir);
    free(path);
};

// TODO
void init_search() {};
Pair_int search_pattern_regex(char *line, size_t len, size_t start) {};