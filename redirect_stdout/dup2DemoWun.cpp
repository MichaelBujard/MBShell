#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    int number1, number2, sum;

    int input_file_descriptors = open("./input.txt", O_RDONLY);

    if (dup2(input_file_descriptors, STDIN_FILENO) < 0) {
        printf("Can't do a dup on the file descriptor given.");
        exit(EXIT_FAILURE);
    }

    scanf("%d %d", &number1, &number2);

    sum = number1 + number2;

    printf("%d + %d = %d\n", number1, number2, sum);

    return EXIT_SUCCESS;
}