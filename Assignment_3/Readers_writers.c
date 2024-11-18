#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2
#define READ_COUNT 3
#define WRITE_COUNT 3

sem_t rw_mutex; // Semaphore for allowing writers
sem_t mutex;    // Semaphore for counting readers
int read_count = 0; // Number of readers currently reading

void* reader(void* id);
void* writer(void* id);

int shared_data = 0; // The shared resource

int main() {
    pthread_t readers[NUM_READERS];
    pthread_t writers[NUM_WRITERS];
    int ids[NUM_READERS > NUM_WRITERS ? NUM_READERS : NUM_WRITERS];

    // Initialize semaphores
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, (void*)&ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, (void*)&ids[i]);
    }

    // Wait for all reader threads to finish
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    // Wait for all writer threads to finish
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    // Cleanup semaphores
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);

    return 0;
}

void* reader(void* id) {
    int reader_id = *(int*)id;

    for (int i = 0; i < READ_COUNT; i++) {
        // Entry section
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&rw_mutex); // First reader locks the writer
        }
        sem_post(&mutex);

        // Critical section (Reading)
        printf("Reader %d: read the shared data as %d\n", reader_id, shared_data);
        sleep(1);

        // Exit section
        sem_wait(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&rw_mutex); // Last reader unlocks the writer
        }
        sem_post(&mutex);

        sleep(1); // Simulate time between readings
    }

    return NULL;
}

void* writer(void* id) {
    int writer_id = *(int*)id;

    for (int i = 0; i < WRITE_COUNT; i++) {
        // Entry section
        sem_wait(&rw_mutex); // Writer locks out readers

        // Critical section (Writing)
        shared_data = rand() % 100; // Write random data to the shared resource
        printf("Writer %d: wrote %d to the shared data\n", writer_id, shared_data);
        sleep(2);

        // Exit section
        sem_post(&rw_mutex); // Writer releases lock

        sleep(2); // Simulate time between writings
    }

    return NULL;
}
