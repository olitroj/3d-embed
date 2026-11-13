all:
	@mkdir -p bin
	gcc main.c -o bin/main

clean:
	rm -rf bin

.PHONY: all clean