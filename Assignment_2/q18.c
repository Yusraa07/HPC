#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
    printf("Environment variables:\n");

    for (char **env = envp; *env != 0; env++) {
        printf("%s\n", *env);
    }

    return 0;
}
