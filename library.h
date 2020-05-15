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





struct threads {
    struct thread_info *head; // primeiro thread a ser criado
    struct thread_info *tail; // utima thread criada

    int n_threads; // numero de threads criadas no total
    char *path; // path to search from, in dept

    char *name, *prefix_name, *sufix_name;
    char *iname, *prefix_iname, *sufix_iname;

    char type; // X means it was not specified
    bool empty; //starts false
    bool executable; //starts false
    int mmin;
    int size;
    bool above_size; //starts false
};

struct thread_info {
    pthread_t thread_id;  //id of the thread

    char **matches; // strings that match the search in this thread
    int n_matches; // numnero de matches encontrados ou seja numero de strings em matches

    struct thread_info *next; // threads created when found a directory, nexy = tail->next
};




void * search(void *param);
int isSocket(const char *path) ;
int isCharater(const char *path);
int isBlock(const char *path);
int isSymbolicLink(const char *path);
int isFile(const char *path);
int isDirectory(const char *path);
int isFifo(const char * path);
char *stringLwr(char *s) ;
int isAboveorUnderSize(int size, bool above_size, const char *path);
int isMmin(int nnmin, const char *path);
int isExecutable(bool executable, const char *path);
int isEmpty(bool empty, const char *path);
int isType(const char type, const char *path);
int isIname(const char * name, const char * suf, const char * pref, char *d_name);
int isName(const char * name,const char * suf,const char * pref,const char *d_name);
void print_struct(struct threads *args);
void inicialize_threads_struct(struct threads *args);
void parse_args(int argc, char *argv[], struct threads *args);
void print_matches(struct threads threads);
