#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char* argv[]) 
{
    int n = argc-1;

    char shm_name[] = "collatz";
    int page_size = getpagesize();
    int shm_size = n * page_size; 
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR , 0666);

    if (ftruncate(shm_fd , shm_size) == -1) 
    {
        perror(NULL);
        shm_unlink(shm_name);
        return errno;
    }

    char *shm_ptr = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    printf("Start parinte (PID = %d)\n", getpid());
    pid_t pid;

    for (int i=0; i<n; i++)
    {
        pid = fork();
        if(pid == 0) 
        {
            int nr = atoi(argv[i+1]), offset = i * page_size;
            while (nr != 1) 
            {
                int written = sprintf(shm_ptr+offset,"%d ",nr);
                offset = offset + written;
                if(nr % 2 == 0)
                    nr /= 2;
                else
                    nr = 3*nr + 1;
            }
            printf("Stop Parinte (PID = %d) Copil (PID = %d)\n" , getppid(), getpid());
            munmap(shm_ptr, shm_size);
            exit(1);
        }
    }
    
    if (pid > 0)
    {
        while(wait(NULL) >0);

        for(int i = 0; i < n; i++) 
        {
            //afisare nr de start
            printf("%s: ",argv[i+1]);
            int offset = i * page_size;
            //afisare numere din ipoteza collatz
            printf("%s 1\n",shm_ptr + i * page_size);
        }
        printf("Stop Parinte (PID = %d) Copil (PID = %d)\n" , getppid(), getpid());
        munmap(shm_ptr, shm_size);
        shm_unlink(shm_name);
    }
}