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
#include <pthread.h>
#include <sys/stat.h>
#include <string.h>


void * search_directory_create_thread_to_search_it(void * param);



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


pthread_mutex_t trinco = PTHREAD_MUTEX_INITIALIZER;

int isSocket(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISSOCK(statbuf.st_mode);
}

int isCharater(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISCHR(statbuf.st_mode);
}

int isBlock(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISBLK(statbuf.st_mode);
}

int isSymbolicLink(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISLNK(statbuf.st_mode);
}

int isFile(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISREG(statbuf.st_mode);
}

int isDirectory(const char *path) {
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

int isFifo(const char * path){
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISFIFO(statbuf.st_mode);

}

char *stringLwr(char *s) {
    int len, i;
    len = strlen(s);

    for (i = 0; i < len; i++) {
        s[i] = tolower(s[i]);

    }
    return s;
}




/**
 * Testa se o ficheiro e maior ou menor (especificado em above_size)
 * size contem o tamanho em Megas
 * Se size estiver a 0 significa que nao foi especificado nos argumentos
 * @param size tamanho
 * @param above_size true para acima de size , false para abaixo de size
 * @param path ca,imho para o ficheiro a testar
 * @return true se estiver nos tamanbos estipulados
 */
int isAboveorUnderSize(int size, bool above_size, const char *path) {
    if (size == 0) return 1; // se nao tiver sido explicito nada nos argumentos deste parametro
    //size expressed in Megabytes
    long bytes_size = size * 1048576;
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    if(above_size == true ){
        return bytes_size > statbuf.st_size;
    }else return bytes_size < statbuf.st_size;
}

/**
 * Testa se o ficheiro foi modificado nos ultmos nnmin minutos
 * Se mmin estiver a 0 significa que nao foi especificado nos argumentos
 * e sendo assim retorna true
 * @param nnmin numero de minutos
 * @param path caminho para o file a testar
 * @return true se o ficheiro tiver sido modificado dentro do tempo modifcado
 */
int isMmin(int nnmin, const char *path) {
    if (nnmin == 0) return 1;  // se nao tiver sido explicito nada nos argumentos deste parametro
    struct stat file_stat;
    int err = stat(path, &file_stat);
    if (err != 0) {
        perror(" [file_is_modified] stat");
        exit(errno);
    }
    struct tm *time_of_mod;
    struct tm *time;
    stat(path, &file_stat);
    time_of_mod = gmtime(&(file_stat.st_mtime)); // obtem o tempo da ultima modificacao do ficheiro
    time = localtime(0); // obtem a hora local
    // obtem o tempo pedido, o ficheiro nao deve ter sido criado antes deste tempo para retornar verdadeiro
    time = (struct tm *) (time->tm_min - nnmin);
    if (time_of_mod > time) return 1; //esta comparacao poder na estar a resultar

    return 0;
}

/**
 * Se executable tiver null significa que nao foi especificado nos args
 * e sendo assim retorna true
 * @param executable parametro especificado em args
 * @param path path para o ficheiro a testar
 * @return true se o ficheiro for executavel
 */
int isExecutable(bool executable, const char *path) {
    if(executable == false) return 1;  // se nao tiver sido explicito nada nos argumentos deste parametro
    struct stat statbuf;
    if (stat(path, &statbuf) == 0 && statbuf.st_mode & S_IXUSR)
        return 1;
    return 0;
}

/**
 * Se empty tiver null significa que nao foi especificado nos args
 * e sendo assim retorna true
 * Testa se o ficheiro passado em path esta vazio ou nao
 * Se estiver vazio retorna true
 *
 * @param empty paramtro especificado em args
 * @param path path para o ficheiro a testar
 * @return true se o ficheiro tiver vazio
 */
int isEmpty(bool empty, const char *path) {
    if (empty == false) return 1;  // se nao tiver sido explicito nada nos argumentos deste parametro
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return statbuf.st_size == 0; // se o tamanho for 0 retorna 1
}

/**
 * Se type tiver x que significa que nao foi especificado nos args
 * e sendo assim retorna true
 * De acordo com o caracter em type testa se o ficheiro incluido em path
 * verifica se
 *
 * @param type tipo escolhido em args
 * @param path path para o ficheiro
 * @return veradadeiro se for do tipo escolhido
 */
int isType(const char type, const char *path) {
    if (type == 'X') return 1 ;  // se nao tiver sido explicito nada nos argumentos deste parametro
    switch (type) {
        case 'c':
            if (isCharater(path)){
                return 1;
            }else return 0;
        case 'b':
            if (isBlock(path)){
                return 1;
            }else return 0;
        case 'd':
            if (isDirectory(path)){
                return 1;
            }else return 0;
        case 'p':
            if (isFifo(path)){
                return 1;
            }else return 0;
        case 'f':
            if (isFile(path)){
                return 1;
            }else return 0;
        case 'l':
            if (isSymbolicLink(path)){
                return 1;
            }else return 0;
        case 's':
            if (isSocket(path)){
                return 1;
            }else return 0;
        default: return 0;
    }
}

/**
 * esta funcao ignora minusculas e maiusculas
 * passa d_name para minuscula visto que name ja foi convertido em arg_parse
 * Se os 3 estiverem a null ele automaticamente retorna true pois nao foi expecificado como argumento
 * Checka se o nome em d_name e o mesmo que name
 * ou se tem sub em name
 * ou se tem pref em name
 * um dos 3 existe os outros dois estao a null
 *
 * @param name parametro
 * @param suf parametro
 * @param pref parametro
 * @param d_name ficheiro em investigacao
 * @return retorna true se um dos 3 elementos checkar com o d_name
 */
int isIname(const char * name, const char * suf, const char * pref, char *d_name) {

    if (name == NULL && suf == NULL && pref == NULL) return 1;  // se nao tiver sido explicito nada nos argumentos deste parametro
    d_name = stringLwr(d_name); // passa o nome do ficheiro encontrado para minuscula
    if (name == d_name) return 1;
    if (pref != NULL){
        // testar se prefixo == a primeira parte de d_name
        for (int i = 0; i < strlen(pref) ; i++) {
            if (d_name[i] != pref[i]){
                return 0; // se for diferente retorna logo falso
            }
        }
        return 1; // se sair fora do for quer dizer que pref e igual as primeiras strlen(pref) posicoes de pref
    }
    // testar se o sufixo  == a ultima parte de d_name
    int last_pos_suf = strlen(suf) -1;
    int last_pos_name = strlen(name) - 1;
    while(last_pos_suf >= 0){
        if (d_name[last_pos_name] != suf[last_pos_suf]){
            return 0; // se for diferente retorna logo falso
        }
        last_pos_name --;
        last_pos_suf --;
    }
    return 1;
}

/**
 * Se os 3 estiverem a null ele automaticamente retorna true pois nao foi expecificado como argumento
 * Checka se o nome em d_name e o mesmo que name
 * ou se tem sub em name
 * ou se tem pref em name
 * um dos 3 existe os outros dois estao a null
 *
 * @param name parametro
 * @param suf parametro
 * @param pref parametro
 * @param d_name ficheiro em investigacao
 * @return retorna true se um dos 3 elementos checkar com o d_name
 */
int isName(const char * name, const char * suf, const char * pref, const char *d_name) {
    if (name == NULL && suf == NULL && pref == NULL) return 1;  // se nao tiver sido explicito nada nos argumentos deste parametro
    if (name == d_name) return 1;
    if (pref != NULL){
        // testar se prefixo == a primeira parte de d_name
        for (int i = 0; i < strlen(pref) ; i++) {
            if (d_name[i] != pref[i]){
                return 0; // se for diferente retorna logo falso
            }
        }
        return 1; // se sair fora do for quer dizer que pref e igual as primeiras strlen(pref) posicoes de pref
    }
    // testar se o sufixo  == a ultima parte de d_name
    int last_pos_suf = strlen(suf) -1;
    int last_pos_name = strlen(name) - 1;
    while(last_pos_suf >= 0){
        if (d_name[last_pos_name] != suf[last_pos_suf]){
            return 0; // se for diferente retorna logo falso
        }
        last_pos_name --;
        last_pos_suf --;
    }
    return 1;
}







void print_struct(struct threads args) {
    printf("path: %s\n", args.path);
    printf("name: %s%s%s\n", args.name, args.sufix_name, args.prefix_name);
    printf("iname: %s%s%s\n", args.iname, args.sufix_iname, args.prefix_iname);
    printf("type: %c\n", args.type);
    printf("empty: %d\n", args.empty);
    printf("executable: %d\n", args.executable);
    printf("mmin: %d\n", args.mmin);
    printf("size: %d above this: %d\n", args.size, args.above_size);
}

void inicialize_threads_struct(struct threads *args) {
    args->head = NULL;
    args->tail = NULL;
    args->path = NULL;
    args->name = NULL;
    args->prefix_name = NULL;
    args->sufix_name = NULL;
    args->iname = NULL;
    args->prefix_iname = NULL;
    args->sufix_iname = NULL;
    args->type = 'X';
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
        char tmp = argv[1][0]; // para checkar se a primeria posicao e um ponto
        if (tmp == '.') {
            if (strlen(argv[1]) == 1){
                args->path = malloc(strlen(path_wd) + 1);
                sprintf(args->path,"%s/",path_wd);
            }else {
                char *path_arg = argv[1];
                memmove(&path_arg[0], &path_arg[1], strlen(path_arg)); //remove a primeira posicao
                args->path = malloc(strlen(path_wd) + strlen(path_arg));
                sprintf(args->path, "%s%s", path_wd, path_arg);
            }
        } else { // se for uma directoria em especifico
            args->path = malloc(strlen(argv[1]));
            args->path = argv[1];
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
                char carecter = (char) argv[1];
                // se for differente de todas nao esta dentro das escolhas para o tipo
                if(carecter != 'c' && carecter != 'b' && carecter != 'd' && carecter != 'p' && carecter != 'f' && carecter != 'l' && carecter != 's' && carecter != 'd' ){
                    printf("type choosed is not acepted\n");
                    exit(EXIT_FAILURE);
                }
                args->type = (char) argv[i + 1];
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

void print_matches(struct threads threads){

    struct thread_info *tmp = threads.head; // tmp fica com a primeria thrad inicializada
    for (int i = 0; i < threads.n_threads ; i++) {

        for (int j = 0; j < tmp->n_matches ; j++) {
            printf("%s\n",*(tmp->matches + j));
        }
        tmp = tmp->next; // tmp fica com a thread inicializada a seguir a mesma
    }
}

