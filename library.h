//
// Created by pedro on 11/05/20.
//

#ifndef MYFIND_LIBRARY_H
#define MYFIND_LIBRARY_H


#endif //MYFIND_LIBRARY_H

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <string.h>


typedef int (*PARAM)(char * value, const char *path, char * d_name);

typedef struct args {
    PARAM opt;
    char *value;
} ARGS;

typedef struct data {
    ARGS args[10];
    char *path; // path to search from, in dept
    int n_args;
}DATA;


typedef struct threads {
    struct matches *matches;
    pthread_t thread_id;  //id of the thread
    struct threads *next; // threads created when found a directory, nexy = tail->next
    int n_matches; // numnero de matches encontrados ou seja numero de strings em matches
} THREADS;



typedef struct matches {
    char *match; // strings that match the search in this thread
    struct matches *next;
} MATCHES;


MATCHES *create_new_match_in_thread(MATCHES *current_match,char *new_path);
THREADS *create_thread_and_match(pthread_t self, char *new_path);

void *search(void *param);
int isSocket(const char *path);
int isCharater(const char *path);
int isBlock(const char *path);
int isSymbolicLink(const char *path);
int isFile(const char *path);
int isDirectory(const char *path);
int isFifo(const char *path);
char *stringLwr(char *s);
void parse_args(int argc, char *argv[], DATA *data);
void print_matches(THREADS *threads);