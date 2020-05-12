
#include "library.h"

int main(int argc, char *argv[]) {
    char path_wd[200];
    getcwd(path_wd, sizeof(path_wd));


    char *path = malloc(sizeof(char) * 300);
    char *name = NULL, *iname = NULL, *type = NULL;
    char *prefix_name = NULL, *sufix_name = NULL;
    char *prefix_iname = NULL, *sufix_iname = NULL;
    bool empty = false, executable = false;
    int mmin = 0, size = 0;
    bool above_size = false;


    if (argc == 1) {
        printf("Nao foram especificados argumentos\n");
    }
    if (argc >= 2) {

        if (strcmp(argv[1], ".") == 0 ) {
            if (strlen(argv[1]) == 1)path = path_wd;
            else {
                char *path_arg = argv[1];
                memmove(&path_arg[0], &path_arg[1], strlen(path_arg)); //remove a primeira posicao
                sprintf(path, "%s%s", path_wd, path_arg);
            }
        } else {
            sprintf(path, "%s%s", path_wd, argv[1]);
        }
        int i = 2;
        while (i < argc) {
            char *test = argv[i];
            if (strcmp(argv[i], "-name") == 0) {
                if (argv[i + 1][0] == '*') {
                    sufix_name = malloc(sizeof(argv[i + 1]) - 1);
                    int iterator = 0;
                    for (int j = 1; j < strlen(argv[i + 1]); j++) {
                        sufix_name[iterator] = argv[i + 1][j];
                        iterator++;
                    }
                }
                    // indica apenas o prefixo do nome
                else if (argv[i + 1][strlen(argv[i + 1]) - 1] == '*') {
                    prefix_name = malloc(sizeof(argv[i + 1]) - 1);
                    int iterator = 0;
                    for (int j = 0; j < strlen(argv[i + 1]) - 1; j++) {
                        prefix_name[iterator] = argv[i + 1][j];
                        iterator++;
                    }
                } else {
                    name = malloc(sizeof(argv[i + 1]));
                    strcpy(name, argv[i + 1]);
                }
                i += 2;



            } else if (strcmp(argv[i], "-iname") == 0) {
                // indica apenas o sufixo do nome
                if (argv[i + 1][0] == '*') {
                    sufix_iname = malloc(sizeof(argv[i + 1]) - 1);
                    int iterator = 0;
                    for (int j = 1; j < strlen(argv[i + 1]); j++) {
                        sufix_iname[iterator] = argv[i + 1][j];
                        iterator++;
                    }
                    sufix_iname = stringLwr(sufix_iname);
                }
                // indica apenas o prefixo do nome
                else if (argv[i + 1][strlen(argv[i + 1]) - 1] == '*') {
                    prefix_iname = malloc(sizeof(argv[i + 1]) - 1);
                    int iterator = 0;
                    for (int j = 0; j < strlen(argv[i + 1]) - 1; j++) {
                        prefix_iname[iterator] = argv[i + 1][j];
                        iterator++;
                    }
                    prefix_iname = stringLwr(prefix_iname);
                } else {
                    iname = malloc(sizeof(argv[i + 1]));
                    strcpy(iname, argv[i + 1]);
                    iname = stringLwr(iname);
                }
                i += 2;

            } else if (strcmp(argv[i], "-type") == 0) {
                type = malloc(sizeof(argv[i + 1]));
                strcpy(type, argv[i + 1]);
                i += 2;
            } else if (strcmp(argv[i], "-empty") == 0) {
                empty = true;
                i++;
            } else if (strcmp(argv[i], "-executable") == 0) {
                executable = true;
                i++;
            } else if (strcmp(argv[i], "-mmin") == 0) {
                char *time = argv[i+1];
                memmove(&time[0], &time[1], strlen(time)); //remove a primeira posicao
                mmin = atoi(time);
                i += 2;
            } else if (strcmp(argv[i], "-size") == 0) {

                if (argv[i+1][0] == '+') above_size = true;
                char *sz = argv[i+1];
                memmove(&sz[0], &sz[1], strlen(sz)); //remove a primeira posicao
                size = atoi(sz);
                i += 2;
            } else {
                printf("Argumento %s nao valido\n", argv[i]);
                break;
            }
        }


    }

    printf("path: %s\n", path);
    printf("name: %s%s%s\n", name, sufix_name, prefix_name);
    printf("iname: %s%s%s\n", iname, sufix_iname, prefix_iname);
    printf("type: %s\n", type);
    printf("empty: %d\n", empty);
    printf("executable: %d\n", executable);
    printf("mmin: %d\n", mmin);
    printf("size: %d above this: %d\n", size, above_size);
    return 0;
}
