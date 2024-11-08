#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 100
#define BITS_PER_BYTE 8
#define MEMORY_SIZE (MEM_SIZE * BITS_PER_BYTE)
#define DEBUG_ITERATIONS 10 

int read_file_into_memory(const char* filename, char* memory) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Cannot open file\n");
        return -1;
    }

    char buf[BITS_PER_BYTE + 2];
    int bitIndex = 0;

    while (fgets(buf, sizeof(buf), fp) && bitIndex < MEMORY_SIZE) {
        for (int i = 0; i < BITS_PER_BYTE && buf[i] != '\0' && buf[i] != '\n'; i++) {
            memory[bitIndex++] = buf[i];
        }
    }

    fclose(fp);
    return bitIndex;
}

int mux(int imm_value, int alu_result, int select) {
    return (select == 0) ? alu_result : imm_value;
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
        *ENA = 0;
        *ENB = 0;
        *ENO = 0;
    }

    printf("Decoded Instruction - ENA=%d, ENB=%d, ENO=%d\n", *ENA, *ENB, *ENO);
}

int extract_imm(const char* instruction) {
    return (instruction[5] - '0') * 4 + (instruction[6] - '0') * 2 + (instruction[7] - '0');
}

int alu(int A, int B, int S, int *carry) {
    int result;
    *carry = 0;
    if (S == 0) {
        result = A + B;
        *carry = (result > 15);
        result &= 0xF;
    } else {
        int B_complement = (~B & 0xF) + 1;
        result = A + B_complement;
        *carry = (result > 15);
        result &= 0xF;
    }
    return result;
}

int process_instruction(const char* instruction, int* RA, int* RB, int* RO, int* carry, int* program_counter) {
    int ENA = 0, ENB = 0, ENO = 0;
    decoder(instruction, &ENA, &ENB, &ENO);

    int imm_value = extract_imm(instruction);
    int sreg = instruction[4] - '0';
    int select = instruction[5] - '0';
    int J = instruction[0] - '0';
    int C = instruction[1] - '0';

    int alu_result = alu(*RA, *RB, select, carry);
    int mux_output = mux(imm_value, alu_result, sreg);

    if (ENA) {
        *RA = mux_output;
        printf("RA = %d (MUX output stored in RA)\n", *RA);
    }

    if (ENB) {
        *RB = mux_output;
        printf("RB = %d (MUX output stored in RB)\n", *RB);
    }

    if (ENO) {
        *RO = *RA;
        printf("RO = %d (ALU result stored in RO)\n", *RO);
    }

    printf("ALU result: %d, Carry: %d (Select signal: %d)\n", alu_result, *carry, select);

    if (J == 1) {
        *program_counter = imm_value;
        printf("Jump to instruction %d\n", *program_counter);
        return 1;
    } else if (C == 1 && *carry == 1) {
        *program_counter = imm_value;
        printf("Conditional Jump to instruction %d due to carry\n", *program_counter);
        return 1;
    }

    return 0;
}

int main() {
    char memory[MEMORY_SIZE];
    int bitIndex = read_file_into_memory("fibonacci.bin", memory);

    if (bitIndex == -1) {
        return 1;
    }

    char instruction[BITS_PER_BYTE + 1];
    instruction[BITS_PER_BYTE] = '\0';

    int RA = 0, RB = 0, RO = 0;
    int carry = 0;
    int program_counter = 0;
    int iteration = 0;

    while (program_counter < bitIndex / BITS_PER_BYTE && iteration < DEBUG_ITERATIONS) {
        int countByte = 0;
        for (int i = 0; i < BITS_PER_BYTE; i++) {
            instruction[i] = memory[program_counter * BITS_PER_BYTE + i];
        }
        instruction[BITS_PER_BYTE] = '\0';

        printf("\nIteration %d - Program Counter: %d\n", iteration + 1, program_counter);
        int jump_made = process_instruction(instruction, &RA, &RB, &RO, &carry, &program_counter);
        
        if (!jump_made) {
            program_counter++;
        }

        printf("After instruction - RA: %d, RB: %d, RO: %d, Carry: %d\n", RA, RB, RO, carry);
        iteration++;
    }

    printf("\nDebugging complete. Program halted after %d iterations.\n", DEBUG_ITERATIONS);
    return 0;
}
