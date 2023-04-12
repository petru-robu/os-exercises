#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

struct threadArg 
{
    char* c;
};

typedef struct threadArg threadArg;

void* threadFunc(void* arg) 
{
    threadArg* a = (threadArg*)arg;
    int l = strlen (a->c);

    char* returnVal = malloc(l*(sizeof(char)));

    for(int i=0; i<l; i++)
        returnVal[i] = a->c[l-i-1];

    return returnVal;
}

int main (int argc, char* argv[]) 
{
    pthread_t thr;
    void * thrRetVal;
    threadArg a;

    if(!argv[1]) {
        printf("Introduceti un sir!");
        return EXIT_FAILURE;
    }
    a.c = argv[1];
    if(pthread_create(&thr, NULL, threadFunc, &a)) 
    {
        perror("Nu s-a creat thread!\n");
        return errno;
    }
    if(pthread_join(thr, &thrRetVal))
    {
        perror("Nu se poate da join!\n");
        return errno;
    }
    printf("%s\n", (char*)thrRetVal);
    return EXIT_SUCCESS;
}
