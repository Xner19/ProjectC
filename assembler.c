#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encode.h"  

#define MAX_LINE_LENGTH 100

int main() {
	// let user enter the filename ending with .asm.
    	char input_filename[100];
    	printf("Enter the name of the input .asm file: "); 
    	scanf("%s", input_filename);

    	// normal checking to check if the file is there or not.
    	FILE *file_in = fopen(input_filename, "r");
    	if (file_in == NULL) {
        perror("Error opening input file");
        return 1;
    }

    char output_filename[100];
    strcpy(output_filename, input_filename);  //name copy inputfilename to outputfilename.
    char *dot_position = strrchr(output_filename, '.');  //find the dot and make null after it.
    if (dot_position != NULL) {
        *dot_position = '\0'; //make it null here
    }
    strcat(output_filename, ".bin");  //copy .bin and paste it at the ned of the outputfilename

    // now open the outputfilename and start writing on it
    FILE *file_out = fopen(output_filename, "wb");
    if (file_out == NULL) {
        perror("Error opening output file");
        fclose(file_in);
        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), file_in)) ;
        char *newline_pos = strchr(line, '\n');
        if (newline_pos) {
            *newline_pos = '\0';
        }

        //go to endcode.c to see our logic of PC instructions. 
        unsigned char machine_code = encode_instruction(line);

        //see if the PC went all the way to FF with out having any instruction, then you will get an error. 
        if (machine_code == 0xFF) {
            fprintf(stderr, "Error: Unknown instruction '%s'\n", line);
            fclose(file_in);
            fclose(file_out);
            return 1;
        }

     // write the value of machine_code to assembler.bin
        fwrite(&machine_code, sizeof(unsigned char), 1, file_out);
    }

    	fclose(file_in);
    	fclose(file_out);
    	printf("assembler completed, output written to %s\n", output_filename);
    	return 0;
}
