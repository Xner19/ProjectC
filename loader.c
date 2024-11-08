#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 100
#define BITS_PER_BYTE 8
#define MEMORY_SIZE (MEM_SIZE * BITS_PER_BYTE)

int mux(int imm_value, int alu_result, int select) {
    return (select == 0) ? imm_value : alu_result;
}

void decoder(const char* instruction, int* ENA, int* ENB, int* ENO) {
    int D1 = instruction[2] - '0';
    int D0 = instruction[3] - '0';

    if (D1 == 0 && D0 == 0) {
        *ENA = 1;
        *ENB = 0;
        *ENO = 0;
    } else if (D1 == 0 && D0 == 1) {
        *ENA = 0;
        *ENB = 1;
        *ENO = 0;
    } else if (D1 == 1 && D0 == 0) {
        *ENA = 0;
        *ENB = 0;
        *ENO = 1;
    } else if (D1 == 1 && D0 == 1) {
        *ENA = 1;
        *ENB = 1;
        *ENO = 1;
    }

    printf("ENA=%d  ENB=%d  ENO=%d\n", *ENA, *ENB, *ENO);
}

int extract_imm(const char* instruction) {
    return (instruction[4] - '0') * 4 + (instruction[5] - '0') * 2 + (instruction[6] - '0');  
}

void process_instruction(const char* instruction, int alu_result, int* RA, int* RB) {
    int ENA = 0, ENB = 0, ENO = 0;


    int imm_value = extract_imm(instruction);

    int select_signal = instruction[7] - '0';  
    int mux_output = mux(imm_value, alu_result, select_signal);

    if (ENA) {
        *RA = mux_output;
        printf("RA = %d (MUX output stored in RA)\n", *RA);
    }

    if (ENB) {
        *RB = mux_output;
        printf("RB = %d (MUX output stored in RB)\n", *RB);
    }

    printf("MUX output: %d (Select signal: %d)\n", mux_output, select_signal);
}

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

    char instruction[BITS_PER_BYTE + 1];
    instruction[BITS_PER_BYTE] = '\0';  

    int countByte = 0;
    int alu_result = 42;  

    int RA = 0, RB = 0;

    for (int i = 0; i < bitIndex; i++) {
        instruction[countByte] = memory[i];
        countByte++;

        if (countByte == BITS_PER_BYTE) {
            process_instruction(instruction, alu_result, &RA, &RB);
            countByte = 0;  
        }
    }

    return 0;
}

