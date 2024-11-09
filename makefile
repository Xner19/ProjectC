CC = gcc
CFLAGS = -Wall

ASSEMBLER_OUTPUT = assembler
LOADER_OUTPUT = loader

assembler.bin: fibonacci.asm assembler.c
	$(CC) $(CFLAGS) assembler.c -o $(ASSEMBLER_OUTPUT)
	./$(ASSEMBLER_OUTPUT) fibonacci.asm assembler.bin

loader: loader.c
	$(CC) $(CFLAGS) loader.c -o $(LOADER_OUTPUT)
	./$(LOADER_OUTPUT)

clean:
	rm -f $(ASSEMBLER_OUTPUT) $(LOADER_OUTPUT) *.bin

