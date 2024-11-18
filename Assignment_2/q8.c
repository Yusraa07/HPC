#include <stdio.h>

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b) { return b != 0 ? a / b : 0; }

int main() {
    int choice, a, b;
    int (*operation)(int, int);

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add\n");
        printf("2. Subtract\n");
        printf("3. Multiply\n");
        printf("4. Divide\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 5) break;

        printf("Enter two integers: ");
        scanf("%d %d", &a, &b);

        switch (choice) {
            case 1: operation = add; break;
            case 2: operation = subtract; break;
            case 3: operation = multiply; break;
            case 4: operation = divide; break;
            default: printf("Invalid choice\n"); continue;
        }

        printf("Result: %d\n", operation(a, b));
    }

    return 0;
}
