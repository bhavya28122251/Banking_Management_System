#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char buffer[256]; // Buffer to hold each line

    // Open the file in read mode
    file = fopen("employee_db.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Read each line and print it to the console
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer); // Display the line
    }

    // Close the file after reading
    fclose(file);
    return 0;
}
