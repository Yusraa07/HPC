#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define EATING_TIMES 3

// States of philosophers
typedef enum { THINKING, HUNGRY, EATING } State;

// Mutex lock for critical section
pthread_mutex_t mutex;
// Semaphores for each philosopher to signal availability of forks
sem_t forks[NUM_PHILOSOPHERS];

// Array to track state of each philosopher
State states[NUM_PHILOSOPHERS];

// Function prototypes
void* philosopher(void* id);
void take_forks(int philosopher_id);
void put_forks(int philosopher_id);
void test(int philosopher_id);

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize mutex and semaphores
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 0);
        states[i] = THINKING;
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, (void*)&ids[i]);
    }

    // Wait for all philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Clean up
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }
    pthread_mutex_destroy(&mutex);

    return 0;
}

void* philosopher(void* id) {
    int philosopher_id = *(int*)id;

    for (int i = 0; i < EATING_TIMES; i++) {
        // Think
        printf("Philosopher %d is thinking...\n", philosopher_id);
        sleep(rand() % 3 + 1);

        // Get hungry and try to eat
        take_forks(philosopher_id);

        // Eat
        printf("Philosopher %d is eating...\n", philosopher_id);
        sleep(rand() % 2 + 1);

        // Finish eating and put forks back
        put_forks(philosopher_id);
    }

    return NULL;
}

void take_forks(int philosopher_id) {
    pthread_mutex_lock(&mutex);
    states[philosopher_id] = HUNGRY;
    printf("Philosopher %d is hungry...\n", philosopher_id);

    // Try to pick up forks
    test(philosopher_id);

    pthread_mutex_unlock(&mutex);

    // Wait for the forks to be available
    sem_wait(&forks[philosopher_id]);
}

void put_forks(int philosopher_id) {
    pthread_mutex_lock(&mutex);
    states[philosopher_id] = THINKING;
    printf("Philosopher %d is putting down forks and thinking...\n", philosopher_id);

    // Test if neighbors can eat now
    test((philosopher_id + 1) % NUM_PHILOSOPHERS);
    test((philosopher_id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS);

    pthread_mutex_unlock(&mutex);
}

void test(int philosopher_id) {
    // Check if this philosopher can eat
    if (states[philosopher_id] == HUNGRY &&
        states[(philosopher_id + 1) % NUM_PHILOSOPHERS] != EATING &&
        states[(philosopher_id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS] != EATING) {
        states[philosopher_id] = EATING;
        sem_post(&forks[philosopher_id]);
    }
}
