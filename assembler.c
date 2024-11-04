#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100

unsigned char encode_instruction(const char *line) {
    if (strcmp(line, "RA = RA + RB") == 0) return 0x00;  // Example encoding
    else if (strcmp(line, "RB = RA + RB") == 0x01) return 0x01;
    return 0xFF;  // 0xFF can signify an unknown instruction
}

int main() {
    // Prompt for input and output filenames
    char input_filename[256];
    printf("Enter the name of the input .asm file: ");
    scanf("%s", input_filename);

    // Open the input .asm file
    FILE *file_in = fopen(input_filename, "r");
    if (file_in == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Prepare the output .bin filename
    char output_filename[256];
    snprintf(output_filename, sizeof(output_filename), "%.*s.bin", (int)(strrchr(input_filename, '.') - input_filename), input_filename);
    FILE *file_out = fopen(output_filename, "wb");
    if (file_out == NULL) {
        perror("Error opening output file");
        fclose(file_in);
        return 1;
    }

    // Read each line from the .asm file, parse, and encode
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file_in)) {
        line[strcspn(line, "\n")] = '\0';

        // Encode the line into an 8-bit machine code
        unsigned char machine_code = encode_instruction(line);

        if (machine_code == 0xFF) {
            fprintf(stderr, "Error: Unknown instruction '%s'\n", line);
            fclose(file_in);
            fclose(file_out);
            return 1;
        }

        fwrite(&machine_code, sizeof(unsigned char), 1, file_out);
    }

    fclose(file_in);
    fclose(file_out);
    printf("Assembler completed successfully. Output written to %s\n", output_filename);
    return 0;
}
