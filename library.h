//
// Created by pedro on 11/05/20.
//

#ifndef MYFIND_LIBRARY_H
#define MYFIND_LIBRARY_H


#endif //MYFIND_LIBRARY_H

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

#include <sys/stat.h>
#include <string.h>

// TODO revr logica implementacao das estruturas
struct threads {
    struct thread_info *current; // thread que esta a pesquisar de momento
    char *path; // path to search from, in dept

    char *name;
    char *prefix_name;
    char *sufix_name;

    char *iname;
    char *prefix_iname;
    char *sufix_iname;

    char *type;
    bool empty; //starts false
    bool executable; //starts false
    int mmin;
    int size;
    bool above_size; //starts false
};

struct thread_info {
    struct thread_info *parent; // threads created when found a directory
    pthread_t id; //id of the thread
    char **matches; // strings that match the search in this thread

    struct thread_info **created; // threads created when found a directory
    int n_threads_created; //number of threads this thread created
};


//function to convert into lowercase
char *stringLwr(char *s) {
    int len, i;
    len = strlen(s);

    for (i = 0; i < len; i++) {
        s[i] = tolower(s[i]);

    }
    return s;
}


void print_struct(struct threads args) {
    printf("path: %s\n", args.path);
    printf("name: %s%s%s\n", args.name, args.sufix_name, args.prefix_name);
    printf("iname: %s%s%s\n", args.iname, args.sufix_iname, args.prefix_iname);
    printf("type: %s\n", args.type);
    printf("empty: %d\n", args.empty);
    printf("executable: %d\n", args.executable);
    printf("mmin: %d\n", args.mmin);
    printf("size: %d above this: %d\n", args.size, args.above_size);
}


void inicialize_threads_struct(struct threads *args) {
    args->current = NULL;
    args->path = NULL;
    args->name = NULL;
    args->prefix_name = NULL;
    args->sufix_name = NULL;
    args->iname = NULL;
    args->prefix_iname = NULL;
    args->sufix_iname = NULL;
    args->type = NULL;
    args->empty = false;
    args->executable = false;
    args->mmin = 0;
    args->size = 0;
    args->above_size = false;
}


void parse_args(int argc, char *argv[], struct threads *args) {

    inicialize_threads_struct(args); //inicializa a estrtura threads

    char path_wd[200];
    getcwd(path_wd, sizeof(path_wd));

    if (argc == 1) {
        printf("Nao foram especificados argumentos\n");
    }
    if (argc >= 2) {

        if (strcmp(argv[1], ".") == 0) {
            if (strlen(argv[1]) == 1)args->path = path_wd;
            else {
                char *path_arg = argv[1];
                memmove(&path_arg[0], &path_arg[1], strlen(path_arg)); //remove a primeira posicao
                args->path = malloc(strlen(path_wd) + strlen(path_arg));
                sprintf(args->path, "%s%s", path_wd, path_arg);
            }
        } else {
            args->path = malloc(strlen(path_wd) + strlen(argv[1]));
            sprintf(args->path, "%s%s", path_wd, argv[1]);
        }
        int i = 2;
        while (i < argc) {
            if (strcmp(argv[i], "-name") == 0) {
                if (argv[i + 1][0] == '*') {
                    args->sufix_name = malloc(sizeof(argv[i + 1]) - 1);
                    int iterator = 0;
                    for (int j = 1; j < strlen(argv[i + 1]); j++) {
                        args->sufix_name[iterator] = argv[i + 1][j];
                        iterator++;
                    }
                }
                    // indica apenas o prefixo do nome
                else if (argv[i + 1][strlen(argv[i + 1]) - 1] == '*') {
                    args->prefix_name = malloc(sizeof(argv[i + 1]) - 1);
                    int iterator = 0;
                    for (int j = 0; j < strlen(argv[i + 1]) - 1; j++) {
                        args->prefix_name[iterator] = argv[i + 1][j];
                        iterator++;
                    }
                } else {
                    args->name = malloc(sizeof(argv[i + 1]));
                    strcpy(args->name, argv[i + 1]);
                }
                i += 2;


            } else if (strcmp(argv[i], "-iname") == 0) {
                // indica apenas o sufixo do nome
                if (argv[i + 1][0] == '*') {
                    args->sufix_iname = malloc(sizeof(argv[i + 1]) - 1);
                    int iterator = 0;
                    for (int j = 1; j < strlen(argv[i + 1]); j++) {
                        args->sufix_iname[iterator] = argv[i + 1][j];
                        iterator++;
                    }
                    args->sufix_iname = stringLwr(args->sufix_iname);
                }
                    // indica apenas o prefixo do nome
                else if (argv[i + 1][strlen(argv[i + 1]) - 1] == '*') {
                    args->prefix_iname = malloc(sizeof(argv[i + 1]) - 1);
                    int iterator = 0;
                    for (int j = 0; j < strlen(argv[i + 1]) - 1; j++) {
                        args->prefix_iname[iterator] = argv[i + 1][j];
                        iterator++;
                    }
                    args->prefix_iname = stringLwr(args->prefix_iname);
                } else {
                    args->iname = malloc(sizeof(argv[i + 1]));
                    strcpy(args->iname, argv[i + 1]);
                    args->iname = stringLwr(args->iname);
                }
                i += 2;

            } else if (strcmp(argv[i], "-type") == 0) {
                args->type = malloc(sizeof(argv[i + 1]));
                strcpy(args->type, argv[i + 1]);
                i += 2;
            } else if (strcmp(argv[i], "-empty") == 0) {
                args->empty = true;
                i++;
            } else if (strcmp(argv[i], "-executable") == 0) {
                args->executable = true;
                i++;
            } else if (strcmp(argv[i], "-mmin") == 0) {
                char *time = argv[i + 1];
                memmove(&time[0], &time[1], strlen(time)); //remove a primeira posicao
                args->mmin = atoi(time);
                i += 2;
            } else if (strcmp(argv[i], "-size") == 0) {

                if (argv[i + 1][0] == '+') args->above_size = true;
                char *sz = argv[i + 1];
                memmove(&sz[0], &sz[1], strlen(sz)); //remove a primeira posicao
                args->size = atoi(sz);
                i += 2;
            } else {
                printf("Argumento %s nao valido\n", argv[i]);
                break;
            }
        }
    }

}

int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

void search_directory_create_thread_to_search_it(struct threads *args) {
    // TODO inicializacao da strcut thread_info
    // TODO colocar current->created = thread_info e atualizar current para este thread
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(args->path)) == NULL)
        perror("opendir() error");
    else {
        printf("contents of root in %s:\n", args->path);
        // enqyanto os directorios de dir nao forem null
        char *local_path = args->path; //path do directorio a ser exploraod pela thread
        while ((entry = readdir(dir)) != NULL) {
            char * new_path = malloc(300);
            if (local_path[strlen(local_path)-1] != '/'){ sprintf(new_path, "%s/%s", local_path,entry->d_name);
            } else sprintf(new_path, "%s%s", local_path,entry->d_name);

            if (isDirectory(new_path) && strcmp(entry->d_name,"..") != 0 && strcmp(entry->d_name,".") != 0) {
                args->path = new_path;

                // TODO criacao de thread passsando arg como ultimo parametro e a mesma funcao como teceiro

                printf("Encontrou directorio %s\n", entry->d_name);
                search_directory_create_thread_to_search_it(args);
            }

            // TODO if de os parametros dos arguementos
                // TODO guardar na struct a string
        }
        closedir(dir);
    }
}




