CC = gcc
CFLAGS = -Wall

OUTPUT = assembler

%.bin: %.asm assembler.c
	$(CC) $(CFLAGS) assembler.c -o $(OUTPUT)
	./$(OUTPUT) $< $@

clean:
	rm -f $(OUTPUT) *.bin

