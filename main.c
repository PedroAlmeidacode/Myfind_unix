


#include "library.h"


pthread_mutex_t trinco = PTHREAD_MUTEX_INITIALIZER;

//PERGUNTAS
// se eu fizer pthread_join na seguir a criar a thread nao estou a fazer paralelismo
// posso fazer lock e so o largar depois de criar a proxima thread e obter
// porque que temos de guardar os macthes , pq nao imporimi los logo ?

int main(int argc, char *argv[]) {

    /* requisito A */
    struct threads *threads = malloc(
            sizeof *threads); // inicaliza a estrutura que vai conter a informacao geral para os threads

    // altera a estrtura principal, nao precisa de locks pois ainda nao foram criados threads
    parse_args(argc, argv, threads); // faz o parse dos argumentos recebidos do terminal e coloca os dentro da estrtura
    //print_struct(threads); // imprime a estrutura principal

    /* ************ */
    /* requsito B */
    // inicializa thread_info
    // nao precisa de locks porque a concorrencia ainda nao comecou
    struct thread_info *threadInfo = malloc(sizeof *threadInfo);
    pthread_mutex_lock(&trinco);
    threads->head = threadInfo;
    threads->tail = threadInfo;
    threads->n_threads++; // atualiza o numero de threads lancadas
    pthread_create(&threadInfo->thread_id, NULL, &search, (void *) threads);

    pthread_join(threadInfo->thread_id, NULL); //espera que a main thread termine

    print_matches(*threads);

    return 0;
}


// o que passo por parametro tem de ser estatico
// se for estatico consigo obter a strcut desta thread
void *search(void *param) {

    // o que se passa por parametro nao precisa de lock
    // coloca localamente a informacao recebida por parametr
    struct threads *threads;
    threads = (struct threads *) param;

    // this results if there were no concurrencya and each thread waited for the next
    struct thread_info *thisThread = threads->tail;
    // retiro o path a procurar nesta thread

    char *path_of_search = malloc(300);
    strcpy(path_of_search,threads->path);

    // so abro novamanete o trinco quando tiver retirado a infomacao estipulada no thread anterior
    pthread_mutex_unlock(&trinco);

    // variaveis locais que serao postas na struct this thread no final
    char **matches = (char **) malloc(20 * sizeof(char *)); //apenas pode obter 20 matches por thread
    int n_matches = 0;

    DIR *dir;
    struct dirent *entry;
    //printf("path of search = %s\n",path_of_search);
    if ((dir = opendir(path_of_search)) == NULL) {
        perror("opendir() error");
        printf("Path : %s\n", path_of_search);
    } else {

        // enqyanto os directorios de dir nao forem null
        while ((entry = readdir(dir)) != NULL) {

            if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "") != 0) {

                char *name_of_file = (char *) malloc(strlen(entry->d_name));
                strcpy(name_of_file, (char*) entry->d_name);

                char *new_path = malloc(strlen(name_of_file) + strlen(path_of_search) + 3);
                path_of_search[strlen(path_of_search) - 1] != '/' ? sprintf(new_path, "%s/%s", path_of_search, name_of_file) : sprintf(new_path, "%s%s", path_of_search, name_of_file);

                //printf("new path = %s\n",new_path);
                // se for directorio cria outro thread para o explorar
                if (isDirectory(new_path)) {
                    struct thread_info *threadInfo = malloc(sizeof *threadInfo);

                    // fecho o trinco
                    pthread_mutex_lock(&trinco);
                    threads->tail->next = threadInfo;   // coloca a thread criadora a apontar para esta
                    threads->tail = threadInfo;         // atualiza a tail para esta thread
                    threads->n_threads++;               // atualiza o numero de threads lancadas
                    threads->path = new_path;           //atualiza o path para a nova thread pesquisar , no novo directorio


                    // lanco thread para explorar este novo path (diretorio)
                    pthread_create(&threadInfo->thread_id, NULL, &search,(void *) threads);
                }

                // nao uso if porque pode haver pesquisa por diretorios
                // caso nao seja pesquisa por diretorio os diretorios sao descartados a primeira funcao do if
                // checka se o ficheiro esta dentro dos parametros estipulados em argv
                if (isType(threads->type, new_path) &&
                    isEmpty(threads->empty, new_path) &&
                    isExecutable(threads->executable, new_path) &&
                    isMmin(threads->mmin, new_path) &&
                    isAboveorUnderSize(threads->size, threads->above_size, new_path) &&
                    isName(threads->name, threads->sufix_name, threads->prefix_name, name_of_file) &&
                    isIname(threads->iname, threads->sufix_iname, threads->prefix_iname, name_of_file)) {

                    *(matches + n_matches) = (char *) malloc(strlen(new_path));
                    *(matches + n_matches) = new_path; // coloca o new_path que corresdponde a um match em matches
                    n_matches++; // incrementa em 1 o numero de macthes
                }

            }
            // coloca a informacao de matches na estrutura
            // nao preciso de trincos pois apenas esta fucao mexe nesta informacao
            thisThread->matches = matches;
            thisThread->n_matches = n_matches;
        }
        closedir(dir);
    }
    pthread_exit(NULL); // termina a thread em questao quando o seu trabalho tiver terminado
}

void print_matches(struct threads threads){
    pthread_mutex_lock(&trinco);
    struct thread_info *tmp = threads.head; // tmp fica com a primeria thrad inicializada
    for (int i = 0; i < threads.n_threads ; i++) {

        for (int j = 0; j < tmp->n_matches ; j++) {
            printf("%s\n",*(tmp->matches + j));
        }
        tmp = tmp->next; // tmp fica com a thread inicializada a seguir a mesma
    }
    pthread_mutex_unlock(&trinco);
}
