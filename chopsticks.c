#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <stdlib.h>


// Define globals
#define X 5 
#define INTHOUGHT 2 
#define NOTEATING 1 
#define EATING 0 
#define LEFT (phnum + 4) % X
#define RIGHT (phnum + 1) % X

int state[X]; 
int philosophers[X] = { 0, 1, 2, 3, 4 }; 

sem_t mutex; 
sem_t S[X];

void test(int phnum) 
{ 
    if (state[phnum] == NOTEATING 
        && state[LEFT] != EATING 
        && state[RIGHT] != EATING) { 
        // state that eating 
        state[phnum] = EATING; 
  
        sleep(3); 
  
        printf("Philosopher %d takes chopsticks %d and %d\n", 
                      phnum + 1, LEFT + 1, phnum + 1); 
  
        printf("Philosopher %d is Eating\n", phnum + 1); 
  
        // Get the non-eating philosophers to eat 
         sem_post(&S[phnum]); 
    } 
}

// take up chopsticks 
void take_fork(int phnum) 
{ 
  
    sem_wait(&mutex); 
  
    // state that hungry 
    state[phnum] = NOTEATING; 
  
    printf("Philosopher %d is not eating\n", phnum + 1); 
  
    // eat if andjacent philosophers are not eating
    test(phnum); 
  
    sem_post(&mutex); 
  
    // Wait for your chance 
    sem_wait(&S[phnum]); 
  
    sleep(2); 
} 
  
// stop using chopsticks 
void put_fork(int phnum) 
{ 
  
    sem_wait(&mutex); 
  
    // state that thinking 
    state[phnum] =INTHOUGHT; 
  
    printf("Philosopher is %d putting chopstick %d and %d down\n", 
           phnum + 1, LEFT + 1, phnum + 1); 
    printf("Philosopher %d is in some deep thinking\n", phnum + 1); 
  
    test(LEFT); 
    test(RIGHT); 
  
    sem_post(&mutex); 
} 
  
void* philospher(void* num) 
{ 
  
    while (1) { 
  
        int* i = num; 
  
        sleep(1); 
  
        take_fork(*i); 
  
        sleep(0); 
  
        put_fork(*i); 
    } 
} 
  
int main() 
{ 
  
    int i; 
    pthread_t thread_id[X]; 
  
    // initialize the semaphores 
    sem_init(&mutex, 0, 1); 
  
    for (i = 0; i < X; i++) 
  
        sem_init(&S[i], 0, 0); 
  
    for (i = 0; i < X; i++) { 
  
        // create philosopher processes 
        pthread_create(&thread_id[i], NULL, 
                       philospher, &philosophers[i]); 
  
        printf("Philosopher %d is in some deep thinking\n", i + 1); 
    } 
  
    for (i = 0; i < X; i++) 
  
        pthread_join(thread_id[i], NULL); 
}   
