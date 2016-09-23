#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char* argv[]){

    int numofstr = 0;
    int i = 0;
    char* vyrazheniye = NULL;
    
    for (i = 1; i < argc; i++)
    {
        numofstr += strlen(argv[i]) + 1;
    }
    
    vyrazheniye = malloc(numofstr * sizeof(char));
    
    
    
    return 0;
}