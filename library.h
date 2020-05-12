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

//function to convert into lowercase
char * stringLwr(char *s){
    int len,i;
    len= strlen(s);

    for(i=0;i<len;i++) {
        s[i] = tolower(s[i]);

    }
    return s;
}






