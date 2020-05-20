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

typedef struct args {
    char *name, *prefix_name, *sufix_name;
    char *iname, *prefix_iname, *sufix_iname;

    char type; // X means it was not specified
    bool empty; //starts false
    bool executable; //starts false
    int mmin;
    int size;
    bool above_size; //starts false
} ARGS;

typedef struct data {
    struct args *arguments;
    char *path; // path to search from, in dept
}DATA;




typedef struct threads {
    struct matches *matches;
    int n_threads; // numero de threads criadas no total
    pthread_t thread_id;  //id of the thread
    struct threads *next; // threads created when found a directory, nexy = tail->next
    int n_matches; // numnero de matches encontrados ou seja numero de strings em matches
} THREADS;



typedef struct matches {
    char *match; // strings that match the search in this thread
    struct matches *next;
} MATCHES;




THREADS *find_my_thread_struct(pthread_t self, THREADS *mainThread);

void *search(void *param);

int isSocket(const char *path);

int isCharater(const char *path);

int isBlock(const char *path);

int isSymbolicLink(const char *path);

int isFile(const char *path);

int isDirectory(const char *path);

int isFifo(const char *path);

char *stringLwr(char *s);

int isAboveorUnderSize(int size, bool above_size, const char *path);

int isMmin(int nnmin, const char *path);

int isExecutable(bool executable, const char *path);

int isEmpty(bool empty, const char *path);

int isType(const char type, const char *path);

int isIname(const char *name, const char *suf, const char *pref, char *d_name);

int isName(const char *name, const char *suf, const char *pref, const char *d_name);

void print_struct(DATA *args);

void inicialize_arguments_struct(ARGS *args);

void parse_args(int argc, char *argv[], DATA * data, ARGS *args);

void print_matches(THREADS *threads);