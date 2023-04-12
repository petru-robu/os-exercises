#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#define S_MAX 100

struct thread_data
{
    int i, j;
    int n;
};

typedef struct thread_data thread_data;

int m, n, p;
int a[S_MAX][S_MAX], b[S_MAX][S_MAX], res[S_MAX][S_MAX];

void* thread_func(void* arg)
{
    int *res = malloc(sizeof(int));
    *res = 0;

    thread_data* data = (thread_data*)arg;
    for(int i=0; i<data->n; i++)
        *res += a[data->i][i]*b[i][data->j];
    
    return res;
}

void input()
{
    printf("Introduceri dimensiunile: \n");
    scanf("%d%d%d",&m,&n,&p);
    printf("Prima matrice: \n");
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            scanf("%d", &a[i][j]);
    printf("A doua matrice: \n");
    for (int i=0; i<n; i++)
        for (int j=0; j<p; j++)
            scanf("%d", &b[i][j]);
}

int main()
{
    input();
    pthread_t threads[S_MAX][S_MAX];

    for(int i=0; i<m; i++)
        for(int j=0; j<p; j++)
        {
            thread_data *arg = malloc(sizeof(arg));
            arg->n = n;
            arg->i = i;
            arg->j = j;

            if(pthread_create(&threads[i][j], NULL, thread_func, arg))
            {
                perror("Nu se poate crea thread!\n");
                return errno;
            }
        }

    void* ret_vals[S_MAX][S_MAX];

    for(int i=0; i<m; i++)
        for(int j=0; j<p; j++)
        {
            if(pthread_join(threads[i][j], &ret_vals[i][j]))
            {
                perror("Nu se poate obtine valoarea!\n");
                return errno;
            }
            res[i][j] = *(int*)ret_vals[i][j];
        }   

    for(int i=0; i<m; i++)
    {
        for(int j=0; j<p; j++)
            printf("%d ", res[i][j]);
        printf("\n");
    }
        


    return EXIT_SUCCESS;
}