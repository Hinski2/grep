#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <regex.h>
#include "utils.h"

// flags
extern bool LINE_NO;       // -n print line number
extern bool IGNORE_CASE;   // -i case insensitive     
extern bool COUNT;         // -c count lines
extern bool RECURSIVE;     // -r search recursively
extern bool USE_REGEX;     // -regex use regex for search
extern bool DEBUG_INPUT;   // -d to print input

// path is a path to currently open file / dir that wi will use
extern char *pattern;
extern size_t pattern_occ_cnt;
regex_t regex_pattern;
Vector_int pi;

void init_search() {
    static bool inited = false;
    if(inited) return;
    inited = true;

    if(USE_REGEX) {
        if(regcomp(&regex_pattern, pattern, REG_EXTENDED)) {
            printf("innapropriate regex pattern\n");
            exit(EXIT_FAILURE);
        }

        return;
    } 

    if(IGNORE_CASE) {
        for(size_t i = 0; pattern[i] != '\0'; i++) 
            pattern[i] = char_to_upper(pattern[i]);
    }

    // compute pi
    int n = strlen(pattern);
    for(int i = 0; i < n; i++) Vector_int_push_back(&pi, 0); 

    for(int i = 1; i < n; i++) {
        int j = pi.vals[i - 1];
        while(j > 0 && pattern[i] != pattern[j])
            j = pi.vals[j - 1];
        
        if(pattern[i] == pattern[j])
            j++;
        
        pi.vals[i] = j;
    }
}

char char_to_upper(char c) {
    if('a' <= c && c <= 'z') c &= ~0x20;
    return c;
}

Pair_int search_pattern_regex(char *line, size_t len, size_t start) {
    init_search();
    regmatch_t match[1];
    match[0].rm_so = start;
    match[0].rm_eo = len;

    if(regexec(&regex_pattern, line, 1, match, REG_STARTEND) == 0) {
        return (Pair_int) {match[0].rm_so, match[0].rm_eo -1};
    } else {
        return (Pair_int) {-1, -1};
    }
}

Pair_int search_pattern_str(char *line, size_t len, size_t start) { // TODO change it on kmp
    init_search();
    
    int j = 0, n = pi.size;
    for(int i = start; i < len; i++) {
        if(IGNORE_CASE && char_to_upper(line[i]) != pattern[j]) j = pi.vals[j];
        else if(!IGNORE_CASE && line[i] != pattern[j]) j = pi.vals[j];
        else j++;

        if(j == n) return (Pair_int){i - n + 1, i};
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
        else if(strcmp("-d", argv[i]) == 0) DEBUG_INPUT = true;
        else if(strcmp("-regex", argv[i]) == 0) USE_REGEX = true;
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
