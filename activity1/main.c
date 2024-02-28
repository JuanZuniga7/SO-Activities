#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

struct timespec start, end;
double elapsed_time, read_time;

// Definición del nodo del HashMap
typedef struct node {
    int key;
    int value;
    struct node *next;
} Node;

// Definición del HashMap
typedef struct HashMap {
    int size;
    Node **buckets;
} HashMap;

// Función para crear un nuevo HashMap
HashMap *createHashMap(const int size) {
    HashMap *map = NULL;
    map = malloc(sizeof(HashMap));
    map->size = size;
    map->buckets = (Node **)malloc(sizeof(Node *) * size);
    for (int i = 0; i < size; i++) {
        map->buckets[i] = NULL;
    }
    return map;
}

// Función para obtener el índice del bucket para una clave
int getBucketIndex(HashMap *map, int key) {
    return key % map->size;
}

// Función para insertar un par clave-valor en el HashMap
void put(HashMap *map, int key, int value) {
    int index = getBucketIndex(map, key);
    Node *newNode = malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
}

// Función para obtener el valor asociado a una clave del HashMap
int get(HashMap *map, int key) {
    int index = getBucketIndex(map, key);
    Node *node = map->buckets[index];
    while (node != NULL) {
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }
    return -1; // Clave no encontrada
}

// Función para imprimir el contenido del HashMap
void printHashMap(HashMap *map) {
    for (int i = 0; i < map->size; i++) {
        Node *node = map->buckets[i];
        while (node != NULL) {
            printf("(%d, %d) ", node->key, node->value);
            node = node->next;
        }
    }
    printf("\n");
}

void freeHashMap(HashMap *map) {
    for (int i = 0; i < map->size; i++) {
        Node *node = map->buckets[i];
        while (node != NULL) {
            Node *next = node->next;
            free(node);
            node = next;
        }
    }
    free(map->buckets);
    free(map);
}


int compare(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}

int main(const int argc, char *argv[]) {
    printf("File: %s open\n", argv[1]); // closing parenthesis added

    clock_gettime(CLOCK_REALTIME, &start);
    FILE *file = fopen(argv[1], "r");
    clock_gettime(CLOCK_REALTIME, &end);

    read_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Read time: %f\n", read_time);

    clock_gettime(CLOCK_REALTIME, &start);
    if(file == NULL) {
        printf("Error: File not found\n");
        return 1;
    }

    int size;
    int *array = NULL;
    fscanf(file, "%d", &size);
    array = (int *)malloc(size * sizeof(int));
    HashMap *most_frequent_map = createHashMap(size);
    long int sum = 0;
    for (int i= 0; i < size; i++) {
        fscanf(file, "%d", &array[i]);
        sum += array[i];
        int current_number = array[i];
        // Check if the number exists in the HashMap
        int current_count = get(most_frequent_map, current_number);
        // If it exists, increment the count
        if (current_count != -1) {
            put(most_frequent_map, current_number, current_count + 1);
        } else {
            // If it doesn't exist, add it with a count of 1
            put(most_frequent_map, current_number, 1);
        }
    }
    int most_frequent_number = -1;
    int max_count = 0;

    for (int i = 0; i < size; i++) {
        Node *node = most_frequent_map->buckets[i];
        while (node != NULL) {
            if (node->value > max_count) {
                max_count = node->value;
                most_frequent_number = node->key;
            }
            node = node->next;
        }
    }
    if (most_frequent_number == -1) printf("There is no most frequent number\n");
    else printf("The most frequent number is %d with a total of %d ocurrences\n", most_frequent_number, max_count);
    freeHashMap(most_frequent_map);
    clock_gettime(CLOCK_REALTIME, &end);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    fclose(file);
    printf("Sum: %ld\n", sum);
    printf("sum & most frequent number time: %f\n", elapsed_time);
    clock_gettime(CLOCK_REALTIME, &start);
    printf("Additional data:\n");
    const long int mean = sum / size;
    printf("The Mean %ld\n", mean);
    int variance = 0;
    for (int i = 0; i < size; i++) {
        variance += (array[i] - mean) * (array[i] - mean);
    }
    printf("Variance %d\n", variance);
    const float standard_deviation = sqrt(variance / (float)size);
    printf("Standard deviation %f\n", standard_deviation);
    qsort(array, size, sizeof(int), compare);
    printf("The Median %d\n", array[size / 2]);
    printf("Additional data time: %f\n", elapsed_time);
    free(array);
}