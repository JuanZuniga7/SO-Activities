#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>

pid_t createProcess(void) {
    pid_t pid = fork();
    if(pid == -1){
        printf("Error: Unable to create a new process\n");
        exit(EXIT_FAILURE);
    }
    return pid;
}

bool validateIsChild(pid_t pid) {
    return pid == getppid();
}

void generateRandomNumbers(int min, int max, int limit) {
    if(min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    if(min < 0 || max < 0) {
        printf("Error: min and max must be positive numbers\n");
        exit(EXIT_FAILURE);
    }
    FILE *file = fopen("random_numbers.txt", "w");
    if(file == NULL) {
        printf("Error: Unable to create file\n");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d\n", limit);
    for(int i = 0; i < limit; i++) {
        int randomNumber = (rand() % (max - min + 1)) + min;
        fprintf(file, "%d\n", randomNumber);
    }
    fclose(file);
}

int readLimit(FILE *file) {
    char buffer[100];
    fgets(buffer, sizeof(buffer), file);
    return atoi(buffer);
}

void error(char *message) {
    printf("Error: %s\n", message);
    exit(EXIT_FAILURE);
}

int readNumbersFromFile(char *fileName, int **numbers) {
    FILE *file = fopen(fileName, "r");
    if(!file) error("Unable to open file \n");
    int i, number, limit;
    limit = readLimit(file);
    *numbers = (int *) malloc(limit * sizeof(int));
    if(*numbers == NULL) error("Unable to allocate memory\n");
    for(i = 0; i < limit; i++) {
        fscanf(file, "%d", &number);
        (*numbers)[i] = number;
        printf("%d\n", number);
    }
    return limit;
}

int sumVectorNumbers(int **vector, int startIndex, int limit){
    return 0;
}

int main(int argc, char *argv[]) {
    pid_t ppid = getpid();
    //generateRandomNumbers(100, 1000, 10000);
    int *numbers;
    int limit = readNumbersFromFile(argv[1], &numbers);
    int delta = limit / 2;
    int firstPipe[2];
    pipe(firstPipe);
    pid_t firstChild = createProcess();
    int secondPipe[2];
    pipe(secondPipe);
    //TODO
    if (!validateIsChild(ppid)) {
        //parent
        pid_t secondChild = createProcess();
        close(firstPipe[1]);
        close(secondPipe[1]);
    }
    if(validateIsChild(ppid)){
        if(firstChild == 0){
            int sum = sumVectorNumbers(&numbers, 1, delta-1){

            }
        }
        if(firstChild > 0){

        }
    }
    return 0;
}
