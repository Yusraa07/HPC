#include <stdio.h>

void findLargestSmallest(int *arr, int size, int *largest, int *smallest) {
    *largest = *arr;
    *smallest = *arr;

    for (int i = 1; i < size; i++) {
        if (*(arr + i) > *largest) {
            *largest = *(arr + i);
        }
        if (*(arr + i) < *smallest) {
            *smallest = *(arr + i);
        }
    }
}

int main() {
    int arr[] = {10, 20, 5, 30, 15};
    int size = sizeof(arr) / sizeof(arr[0]);
    int largest, smallest;

    findLargestSmallest(arr, size, &largest, &smallest);

    printf("Largest: %d\n", largest);
    printf("Smallest: %d\n", smallest);

    return 0;
}
