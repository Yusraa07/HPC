#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int isInteger(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (!isInteger(argv[i])) {
            printf("Error: %s is not an integer.\n", argv[i]);
            return 1;
        }
    }

    printf("All arguments are valid integers.\n");

    return 0;
}
