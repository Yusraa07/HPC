#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10  // Size of the buffer
#define PRODUCERS 3     // Number of producer threads
#define CONSUMERS 3     // Number of consumer threads
#define ITEMS_TO_PRODUCE 30  // Number of items each producer will produce

// Buffer structure
typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    int in;
    int out;
} Buffer;

Buffer sharedBuffer = { .count = 0, .in = 0, .out = 0 };

// Mutex and condition variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;

// Producer function
void* producer(void* param) {
    int id = *((int*)param);
    for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
        int item = rand() % 100;  // Produce a random item

        // Acquire the mutex lock
        pthread_mutex_lock(&mutex);

        // Wait if buffer is full
        while (sharedBuffer.count == BUFFER_SIZE) {
            pthread_cond_wait(&notFull, &mutex);
        }

        // Add item to buffer
        sharedBuffer.buffer[sharedBuffer.in] = item;
        sharedBuffer.in = (sharedBuffer.in + 1) % BUFFER_SIZE;
        sharedBuffer.count++;

        printf("Producer %d: Produced item %d\n", id, item);

        // Signal that buffer is not empty
        pthread_cond_signal(&notEmpty);

        // Release the mutex lock
        pthread_mutex_unlock(&mutex);

        sleep(1);  // Simulate time to produce item
    }
    return NULL;
}

// Consumer function
void* consumer(void* param) {
    int id = *((int*)param);
    while (1) {
        // Acquire the mutex lock
        pthread_mutex_lock(&mutex);

        // Wait if buffer is empty
        while (sharedBuffer.count == 0) {
            pthread_cond_wait(&notEmpty, &mutex);
        }

        // Remove item from buffer
        int item = sharedBuffer.buffer[sharedBuffer.out];
        sharedBuffer.out = (sharedBuffer.out + 1) % BUFFER_SIZE;
        sharedBuffer.count--;

        printf("Consumer %d: Consumed item %d\n", id, item);

        // Signal that buffer is not full
        pthread_cond_signal(&notFull);

        // Release the mutex lock
        pthread_mutex_unlock(&mutex);

        sleep(1);  // Simulate time to consume item
    }
    return NULL;
}

int main() {
    pthread_t producerThreads[PRODUCERS];
    pthread_t consumerThreads[CONSUMERS];
    int producerIds[PRODUCERS];
    int consumerIds[CONSUMERS];

    // Create producer threads
    for (int i = 0; i < PRODUCERS; i++) {
        producerIds[i] = i + 1;
        pthread_create(&producerThreads[i], NULL, producer, &producerIds[i]);
    }

    // Create consumer threads
    for (int i = 0; i < CONSUMERS; i++) {
        consumerIds[i] = i + 1;
        pthread_create(&consumerThreads[i], NULL, consumer, &consumerIds[i]);
    }

    // Wait for producer threads to finish
    for (int i = 0; i < PRODUCERS; i++) {
        pthread_join(producerThreads[i], NULL);
    }

    // Since consumers run indefinitely, we'll not join them
    // Instead, they can be cancelled when the program ends

    // Optional: Cancel consumer threads if running indefinitely
    for (int i = 0; i < CONSUMERS; i++) {
        pthread_cancel(consumerThreads[i]);
    }

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&notFull);
    pthread_cond_destroy(&notEmpty);

    return 0;
}
