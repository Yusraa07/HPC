#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CHAIRS 5   // Number of waiting chairs
#define NUM_CUSTOMERS 10 // Number of customers

sem_t waitingRoom; // Semaphore for waiting room chairs
sem_t barberChair; // Semaphore for the barber chair
sem_t barberPillow; // Semaphore to wake up the barber
sem_t seatBelt; // Semaphore to make the customer wait until the haircut is done

int allDone = 0; // Flag to indicate all customers are done

void* customer(void* num);
void* barber(void*);

int main() {
    pthread_t btid;
    pthread_t tid[NUM_CUSTOMERS];
    int customerNum[NUM_CUSTOMERS];

    // Initialize semaphores
    sem_init(&waitingRoom, 0, NUM_CHAIRS);
    sem_init(&barberChair, 0, 1);
    sem_init(&barberPillow, 0, 0);
    sem_init(&seatBelt, 0, 0);

    // Create barber thread
    pthread_create(&btid, NULL, barber, NULL);

    // Create customer threads
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customerNum[i] = i;
        pthread_create(&tid[i], NULL, customer, (void*)&customerNum[i]);
        sleep(1); // Space out customer arrivals
    }

    // Wait for all customer threads to finish
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(tid[i], NULL);
        sleep(1);
    }

    // When all customers are done
    allDone = 1;
    sem_post(&barberPillow); // Wake the barber so he can exit

    // Wait for the barber thread to finish
    pthread_join(btid, NULL);

    // Cleanup semaphores
    sem_destroy(&waitingRoom);
    sem_destroy(&barberChair);
    sem_destroy(&barberPillow);
    sem_destroy(&seatBelt);

    return 0;
}

void* barber(void* junk) {
    while (!allDone) {
        // Sleep until a customer arrives
        printf("The barber is sleeping.\n");
        sem_wait(&barberPillow);

        if (!allDone) {
            // Take a seat in the barber chair
            sem_wait(&barberChair);

            // Cut hair
            printf("The barber is cutting hair.\n");
            sleep(2);

            // Release the barber chair
            sem_post(&barberChair);

            // Signal the customer that the haircut is done
            sem_post(&seatBelt);
        }
        else {
            printf("The barber is going home for the day.\n");
        }
    }
    return NULL;
}

void* customer(void* number) {
    int num = *(int*)number;

    // Try to get a seat in the waiting room
    if (sem_trywait(&waitingRoom) == 0) {
        printf("Customer %d is waiting.\n", num);

        // Wait for the barber chair to be available
        sem_wait(&barberChair);

        // Sit in the barber chair and signal the barber
        sem_post(&waitingRoom); // Leave the waiting room chair
        printf("Customer %d is waking the barber.\n", num);
        sem_post(&barberPillow);

        // Wait for the haircut to be done
        sem_wait(&seatBelt);

        // Release the barber chair
        sem_post(&barberChair);
        printf("Customer %d is leaving after the haircut.\n", num);
    }
    else {
        // No waiting chairs available
        printf("Customer %d is leaving, no chairs available.\n", num);
    }
    return NULL;
}
