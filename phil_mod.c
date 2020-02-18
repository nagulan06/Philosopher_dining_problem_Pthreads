#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

int state[3]={2};
pthread_t* threads;
pthread_mutex_t* lock;
int N; //number of threads(philosophers)
int total;
pthread_cond_t* cond;
int *arr;
int temp = 0;
void *phil(void* i)
{
    while(1)
    {
    int n = *((int *)i);
    int left = n;       //calculate the left and right fork numbers
    int right = (n+1) % total;  //as the table is round the right fork of the last philopher is the first fork
    
    sleep(1+ rand() % 5);   //sleep for some random time

    printf("Philosopher %d is hungry\n", n+1);  //the philosopher state changes from thinking to hungry
    state[n] = 0;
    if(state[n] == 0 && state[left]!=1 && state[right]!=1)  //once hungry, chck if the right and left forks are available to use
    {
        pthread_mutex_lock(&lock[left]);    //if the left and t=right fork is free, lock the forks and start eating
        pthread_mutex_lock(&lock[right]);
        state[n] == 1;  //eating
        temp = 1;
        printf("Philospher %d is eating with fork %d and fork %d\n", n+1, left+1, right+1);
        sleep(1+rand()%5);      //eat for a random time
        printf("Philospher %d puts down fork %d and fork %d\n and is now thinking\n", n+1, left+1, right+1);
//once done eating, put down the fork and unlock the mutex in thr order it was locked so that it becomes available for other threads.
        pthread_mutex_unlock(&lock[right]);
        pthread_mutex_unlock(&lock[left]);
    }
    }
    return NULL;
}

   int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        puts("Usage: ./a.out <Number of Philosophers>\n");  //print usage message and exit if the number of philosophers is not entered properly.
        return(1);
    }
    N = atoi(argv[1]);
    total = N;
    threads = (pthread_t*)malloc(N*sizeof(pthread_t));  //initializing threads
    arr = (int*)malloc(N*sizeof(int));
    lock = (pthread_mutex_t*)malloc(N*sizeof(pthread_mutex_t)); //array of lock variables for mutex locks
    for(int i=0; i < N; i++)
    {
   if(pthread_mutex_init(&lock[i], NULL) != 0)  //initialize the mutex variables.
       puts("Mutex init has failed\n");
    }
    
    for(int i = 0; i < N; i++)
    {  
        arr[i] = i;
        printf("Philosopher %d is thinking \n", i+1);   //the philosophers are initially in the thinking mode
        pthread_create(&threads[i], NULL, &phil, &arr[i]);  //the threads are created with each philospher as a thread and the function is called
    }
// once the thread returns, they are joined
 for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);
 return 0;
}
