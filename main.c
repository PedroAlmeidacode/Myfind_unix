#include "library.h"


pthread_mutex_t trinco = PTHREAD_MUTEX_INITIALIZER;

THREADS *head = NULL; // vai guardar a primeria posicao da linked list threads
int n_threads = 0; // variavel global que guarda o numero de struct thread criadas (nao o numero de threads lancadas)

//PERGUNTAS
// duas thread estao a ir buscar o mesmo path
int main(int argc, char *argv[]) {

    pthread_t thread_id;
    DATA data = {.args = {NULL, ""}, .n_args=0};

    /* requisito A */
    // altera a estrtura principal, nao precisa de locks pois ainda nao foram criados threads
    parse_args(argc, argv, &data); // faz o parse dos argumentos recebidos do terminal e coloca os dentro da estrtura
    /* ************ */
    /* requsito B */
    pthread_create(&thread_id, NULL, &search, (void *) &data);
    pthread_join(thread_id, NULL); //espera que a main thread termine

    print_matches(head);
}


// so e criada a struct da thread se esta tiver um match correpondente ao pedido
void *search(void *param) {


    // instancia  a mainThread
    struct data *data;
    data = (struct data *) param;
    char *path_to_explore = data->path;
    pthread_t ids[200];
    int i_ids = 0;
    int j;


    // retiro o path a procurar nesta thread
    char *path_of_search = malloc(300); // 300 for the record
    strcpy(path_of_search, path_to_explore);


    // structs que posssam vir a ser alocadas
    struct threads *newThread = NULL; //para o caso da thread ainda nao existir se ja existir procuro o coloco nesta variavel
    struct matches *newMatch = NULL; //para o caso de ter um match ou mais
    int n_matches = 0;


    DIR *dir;
    struct dirent *entry;


    // abre directorio para explorar contreudo de primeiro nivel
    if ((dir = opendir(path_of_search)) == NULL) {
        perror("opendir() error");
        pthread_exit(NULL); // termina a thread em questao quando o seu trabalho tiver terminado
    } else {
        // enqyanto os directorios de dir nao forem null
        while ((entry = readdir(dir)) != NULL) {

            //printf("entrada d_name = %s\n",entry->d_name);

            if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "") != 0) {

                char *name_of_file = (char *) malloc(50);
                strcpy(name_of_file, (char *) entry->d_name);

                char *new_path = malloc(300);
                if (path_of_search[strlen(path_of_search) - 1] != '/') {
                    sprintf(new_path, "%s/%s", path_of_search,
                            name_of_file);
                } else {
                    new_path = strcat(path_of_search, name_of_file);
                }

                // se for directorio cria outro thread para o explorar
                if (isDirectory(new_path)) {

                    DATA thread_data[200];
                    (thread_data[i_ids]).path = malloc(300);
                    strcpy((thread_data[i_ids]).path, new_path);
                    thread_data[i_ids].n_args = data->n_args;

                    // copy the rray of args to the new struct
                    for (int i = 0; i < data->n_args; i++) {
                        thread_data[i_ids].args[i].opt = data->args[i].opt;
                        thread_data[i_ids].args[i].value = data->args[i].value;
                    }
                    // lanco thread para explorar este novo path (diretorio)
                    pthread_create(&ids[i_ids], NULL, &search, &thread_data[i_ids]);
                    i_ids++;
                }
                //chama as funcoes de possivel match de acordo com os argumentos definidos
                for (j = 0; j < data->n_args; j++) {
                    if (!data->args[j].opt(data->args[j].value, new_path, name_of_file)) {
                        break;
                    }
                }
                // Match
                if (j == data->n_args) { // se todas as funcoes passarem nos testes j sera igual ao numero de argumentos definidos
                    if (n_matches == 0) {
                        // retorna a struct existente se ela existir, cria strcuct match se ela nao existir
                        // se ela nao existir cria a e cria a strcut match dando os devidos paramnetros
                        newThread = create_thread_and_match(pthread_self(), new_path);
                        newMatch = newThread->matches;
                    } else {
                        // neste caso ja temos as structs criadas
                        //temos de ir a ultima posicao de match para criar outra struct match
                        newMatch = create_new_match_in_thread(newMatch, new_path);
                        //atualiza numero de matches
                        newThread->n_matches++;
                    }
                    n_matches++;
                }
            }
        }
        closedir(dir);
    }

    for (int i = 0; i < i_ids; i++) {
        pthread_join(ids[i], NULL);
    }
    pthread_exit(NULL); // termina a thread em questao quando o seu trabalho tiver terminado
}

void print_matches(THREADS *threads) {
    if (threads == NULL) {
        printf("\tNao existem resultados para a sua pesquisa\n");
    } else {
        printf("Result : \n");
        pthread_mutex_lock(&trinco);
        THREADS *tmp = threads; // tmp fica com a primeria thrad inicializada
        for (int i = 0; i < n_threads; i++) {
            MATCHES *mat = tmp->matches; // tmp fica com a primeria thrad inicializada
            for (int j = 0; j < tmp->n_matches; j++) {
                printf("%s\n", mat->match);
                mat = mat->next;
            }
            tmp = tmp->next; // tmp fica com a thread inicializada a seguir a mesma
        }
        pthread_mutex_unlock(&trinco);
    }
}


THREADS *create_thread_and_match(pthread_t self, char *new_path) {

    MATCHES *newMatch = malloc(sizeof *newMatch);
    newMatch->next = NULL;
    newMatch->match = new_path; // colocar o match na string

    THREADS *newThread = malloc(sizeof *newThread);
    newThread->thread_id = self;
    newThread->next = NULL;
    newThread->n_matches = 1;
    newThread->matches = newMatch; // the head of the matches

    pthread_mutex_lock(&trinco);
    n_threads++;
    if (head ==
        NULL) { // no caso de ainda nao ter sido criada nenhuma estrutura esta primeira ficara assigned a variavel global head
        head = newThread;

    } else {
        THREADS *current = head;
        THREADS *next = current->next;
        // procura a ultima posicao da linked list
        while (next != NULL) {
            current = next;
            next = current->next;
        }
        current->next = newThread; // proxima posicao NULL igual ao novo thread
    }
    pthread_mutex_unlock(&trinco);

    return newThread;
}

MATCHES *create_new_match_in_thread(MATCHES *current_match, char *new_path) {
    MATCHES *match = malloc(sizeof *match);
    match->next = NULL;
    match->match = new_path;
    current_match->next = match;
    return match;
}