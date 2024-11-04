#include <string.h>

// Function to encode an instruction line into machine code (8-bit binary)
unsigned char encode_instruction(const char *line) {
    	if (strcmp(line, "RA = RA + RB") == 0) return 0x00;         // RA = RA + RB
								    
								    
    	else if (strcmp(line, "RB = RA + RB") == 0x01) return 0x01; // RB = RA + RB
								    

    	else if (strcmp(line, "RA = RA - RB") == 0x02) return 0x02; // RA = RA - RB

									
   	else if (strcmp(line, "RB = RA - RB") == 0x03) return 0x03; // RB = RA - RB

    	else if (strcmp(line, "RO = RA") == 0x04) return 0x04;      // RO = RA

    	else if (strncmp(line, "RA = ", 5) == 0) {
        int imm_value = atoi(line + 5);                         // Get immediate value
        if (imm_value >= 0 && imm_value <= 7)                   // Check if within 3-bit range
            return 0x08 | imm_value;                            // RA = imm (0x08 prefix)
    	}	 
	
	else if (strncmp(line, "RB = ", 5) == 0) {
        int imm_value = atoi(line + 5);
        if (imm_value >= 0 && imm_value <= 7)
        return 0x09 | imm_value;                            // RB = imm (0x09 prefix)
    }

    // Jump instructions
    else if (strncmp(line, "JC = ", 5) == 0) {
        int jump_addr = atoi(line + 5);
        if (jump_addr >= 0 && jump_addr <= 7)
            return 0x1C | jump_addr;                            // JC = imm (0x1C prefix)
    } else if (strncmp(line, "J = ", 4) == 0) {
        int jump_addr = atoi(line + 4);
        if (jump_addr >= 0 && jump_addr <= 7)
            return 0x1D | jump_addr;                            // J = imm (0x1D prefix)
    }

    // Unknown instruction
    return 0xFF;
}
