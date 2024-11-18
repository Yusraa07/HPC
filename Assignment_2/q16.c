#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error: Please provide exactly two arguments.\n");
        return 1;
    }

    printf("Arguments are valid: %s, %s\n", argv[1], argv[2]);

    return 0;
}
