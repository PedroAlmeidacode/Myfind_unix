
#include "library.h"




int main(int argc, char *argv[]) {

    /* requisito A */
    struct threads *args = malloc(sizeof *args); // inicaliza a estrutura que vai conter a informacao geral para os threads
    // TODO . -> significa de onde esta , qualquer coisa que nao comece por ponto e um sitio especifico

    // altera a estrtura principal, nao precisa de locks pois ainda nao foram criados threads
    parse_args(argc,argv,args); // faz o parse dos argumentos recebidos do terminal e coloca os dentro da estrtura

    print_struct(*args); // imprime a estrutura principal
    /* ************ */

    /* requsito B */
    args->path = "/home/pedro/Desktop/Myfind_unix/"; //just for test

    // TODO criacao main thread que chama a funao abaixo
    search_directory_create_thread_to_search_it(args);


    return 0;


}


