

#include "library.h"




int main(int argc, char *argv[]) {

    /* requisito A */
    struct threads *threads = malloc(sizeof *threads); // inicaliza a estrutura que vai conter a informacao geral para os threads

    // altera a estrtura principal, nao precisa de locks pois ainda nao foram criados threads
    parse_args(argc,argv,threads); // faz o parse dos argumentos recebidos do terminal e coloca os dentro da estrtura

    print_struct(*threads); // imprime a estrutura principal
    /* ************ */

    /* requsito B */

    // inicializa thread_info
    // nao precisa de locks porque a concorrencia ainda nao comecou
    struct thread_info *threadInfo = malloc(sizeof *threadInfo);
    threads->head = threadInfo;
    threads->tail = threadInfo;
    threads->n_threads++; // atualiza o numero de threads lancadas
    pthread_create(&threadInfo->thread_id, NULL, &search_directory_create_thread_to_search_it, (void *) threads);

    pthread_join(threadInfo->thread_id, NULL);




    // TODO check wrong type in args
    return 0;


}


// o que passo por parametro tem de ser estatico
// se for estatico consigo obter a strcut desta thread
void * search_directory_create_thread_to_search_it(void * param) {

    // o que se passa por parametro nao precisa de lock
    // coloca localamente a informacao recebida por parametro
    struct threads * threads;
    threads = (struct threads*) param;

    // TODO procurar em head to tail de threads una que tenha este thread_id


    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(threads->path)) == NULL) {
        perror("opendir() error");
        printf("Path : %s\n", threads->path);
    }else {
        // enqyanto os directorios de dir nao forem null
        char *local_path = threads->path; //path do directorio a ser exploraod pela thread
        while ((entry = readdir(dir)) != NULL) {

            char * new_path = malloc(300);
            if (local_path[strlen(local_path)-1] != '/'){ sprintf(new_path, "%s/%s", local_path,entry->d_name);
            } else sprintf(new_path, "%s%s", local_path,entry->d_name);

            if (isDirectory(new_path) && strcmp(entry->d_name,"..") != 0 && strcmp(entry->d_name,".") != 0) {


                struct thread_info *threadInfo = malloc(sizeof *threadInfo);

                // fecho o trinco
                pthread_mutex_lock(&trinco);
                threads->tail->next = threadInfo;   // coloca a thread criadora a apontar para esta
                threads->tail = threadInfo;         // atualiza a tail para esta thread
                threads->n_threads++;               // atualiza o numero de threads lancadas
                threads->path = new_path;           //atualiza o path para a nova thread pesquisar , no novo directorio
                // abro o trinco
                pthread_mutex_unlock(&trinco);


                printf("Encontrou directorio %s\n", entry->d_name);
                // lanco thread para explorar este novo path (diretorio)
                pthread_create(&threadInfo->thread_id, NULL, &search_directory_create_thread_to_search_it, (void *) threads);
                pthread_join(threadInfo->thread_id, NULL);


            }
            // nao uso if porque pode haver pesquisa por diretorios
            // caso nao seja pesquisa por diretorio os diretorios sao descartados a primeira funcao do if
            // checka se o ficheiro esta dentro dos parametros estipulados em argv
            if(isType(threads->type, new_path) &&
                isName(threads->name, threads->sufix_name, threads->prefix_name, entry->d_name) &&
                isIname(threads->iname, threads->sufix_iname, threads->prefix_iname, entry->d_name) &&
                isEmpty(threads->empty, new_path) &&
                isExecutable(threads->executable, new_path) &&
                isMmin(threads->mmin, new_path) &&
                isAboveorUnderSize(threads->size, threads->above_size, new_path)){

                // TODO envolver por locks
                // TODO adicionar match a struct info desta thread


                // TODO checkar funcionamento das funcoes em separado vendo se resultam como esperado



            }



        }
        closedir(dir);
    }
}

