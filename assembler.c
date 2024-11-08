#include <stdio.h>
#include <string.h>
#include <ctype.h>

void convtobin(const char* command, char* temp) {
    if (strcmp(command, "RA=RA+RB") == 0) strcpy(temp, "00000000");
    else if (strcmp(command, "RB=RA+RB") == 0) strcpy(temp, "00010000");
    else if (strcmp(command, "RA=RA-RB") == 0) strcpy(temp, "00000100");
    else if (strcmp(command, "RB=RA-RB") == 0) strcpy(temp, "00010100");
    else if (strcmp(command, "RO=RA") == 0) strcpy(temp, "00100000");
    else if (strcmp(command, "RA=0") == 0) strcpy(temp, "00001000");
    else if (strcmp(command, "RB=1") == 0) strcpy(temp, "00011001");
    else if (strcmp(command, "JC=0") == 0) strcpy(temp, "01110000");
    else if (strcmp(command, "J=2") == 0) strcpy(temp, "10110010");
    else strcpy(temp, "11111111"); // Default for unknown commands
}

void Assembler(const char* filename) {
    	FILE *fi = fopen(filename, "r");
    	if (fi == NULL) {
        	printf("Error: Could not open input file %s.\n", filename);
        	return;
    	}

    	char outputFilename[50];
    	strncpy(outputFilename, filename, strlen(filename) - 4);
    	outputFilename[strlen(filename) - 4] = '\0';
    	strcat(outputFilename, ".bin");

    	FILE *fo = fopen(outputFilename, "w");
    	if (fo == NULL) {
        	printf("Error: Could not open output file %s.\n", outputFilename);
        	fclose(fi);
        	return;
    	}

    	char line[100];
    	char cleanedLine[100];
    	char temp[9];

    
    	while (fgets(line, sizeof(line), fi)) {
        	int j = 0;
        	memset(cleanedLine, 0, sizeof(cleanedLine));
        
	
		char *semicolonPos = strchr(line, ';');
        	if (semicolonPos != NULL) {
            	*semicolonPos = '\0';  
        	}
        
	
	for (int i = 0; line[i] != '\0'; i++) {
        
	    
		if (line[i] != ' ' && line[i] != '\n') {
                	cleanedLine[j++] = toupper(line[i]);
            	}
        }
        	cleanedLine[j] = '\0'; 
        	convtobin(cleanedLine, temp);
        	fprintf(fo, "%s\n", temp);
    }
    	fclose(fi);
    	fclose(fo);
    	printf("Conversion completed. Output saved to %s\n", outputFilename);
}

int main() {
    	char filename[50];
    	printf("Please enter the name of the .asm file:\n");
    	fgets(filename, sizeof(filename), stdin);

    	filename[strcspn(filename, "\n")] = '\0';

    	printf("Input file: %s\n", filename);
    	Assembler(filename);

    	return 0;
}

