#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <ENV_VAR_NAME>\n", argv[0]);
        return 1;
    }

    char *value = getenv(argv[1]);

    if (value != NULL) {
        printf("%s: %s\n", argv[1], value);
    } else {
        printf("Environment variable %s not found.\n", argv[1]);
    }

    return 0;
}
