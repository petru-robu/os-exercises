#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void collatz(int n)
{
    while(n!=1)
    {
        printf("%d ", n);
        if(n%2 == 0)
            n = n/2;
        else
            n = 3*n+1;
    }
    printf("1\n");
}

int main(int argc, char** argv)
{
    if(argc<=1)
    {
        printf("Introduceti un nr!\n");
        return EXIT_FAILURE;
    }
    
    for(int i=1; i<argc; i++)
    {
        pid_t pid = fork();
        if(pid < 0)
        {
            printf("Eroare! (PID = %d)\n", pid);
            return EXIT_FAILURE;
        }
        else if(pid == 0)
        {   
            printf("Child (PID = %d) started\n", getpid());
            collatz(atoi(argv[i]));
            printf("Child (PID = %d)finished\n", getpid());
            exit(1);
        }
    }

    for(int i=1; i<argc; i++)
        wait(NULL);
}