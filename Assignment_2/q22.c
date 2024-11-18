#include <stdio.h>
#include <stdlib.h>

int main() {
    char *str;
    int length;

    printf("Enter the length of the string: ");
    scanf("%d", &length);

    str = (char *)malloc((length + 1) * sizeof(char));  // Allocate memory for the string

    if (str == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter a string: ");
    scanf(" ");
    fgets(str, length + 1, stdin);  // Read string from user

    printf("You entered: %s\n", str);

    free(str);  // Free the allocated memory

    return 0;
}
