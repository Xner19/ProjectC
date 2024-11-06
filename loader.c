#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 100
#define BITS_PER_BYTE 8
#define MEMORY_SIZE (MEM_SIZE * BITS_PER_BYTE)  

int main() {
    FILE *fp = fopen("fibonacci.bin", "r"); 
    if (fp == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    char buf[100];                 
    char memory[MEMORY_SIZE];     
    int bitIndex = 0;             
    int counter = 0;              

    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = '0';
    }

    while (fgets(buf, sizeof(buf), fp)) {
        for (int i = 0; buf[i] != '\0' && buf[i] != '\n' && counter < BITS_PER_BYTE; i++) {
            memory[bitIndex] = buf[i];
            bitIndex++;
            counter++;
        }

        if (counter == BITS_PER_BYTE) {
            counter = 0;
        }
    }

    fclose(fp);

    printf("\nStored instructions:\n");
    for (int i = 0; i < bitIndex; i += BITS_PER_BYTE) {
        printf("Instruction %d: ", i / BITS_PER_BYTE);
        for (int j = 0; j < BITS_PER_BYTE; j++) {
            printf("%c", memory[i + j]);
        }
        printf("\n");
    }

    return 0;
}
