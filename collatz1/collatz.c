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
    if(argc <= 1)
    {
        printf("Introdu un nr la argument!\n");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if(pid<0)
    {
        printf("Eroare! (PID = %d)\n", pid);
        return EXIT_FAILURE;
    }   
    else if(pid == 0)
    {
        printf("Child (PID = %d) started from (PID = %d)\n", getpid(), getppid());
        collatz(atoi(argv[1]));
    }
    else
    {
        wait(NULL);
        printf("Child (PID = %d) finsihed. Parent (PID = %d) finshed.\n", pid, getpid());
    }

}