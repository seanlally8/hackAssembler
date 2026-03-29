hackAssembler: src/parser.c src/helper.c src/code.c src/hackAssembler.c 
	gcc -std=c99 -Wall -Wextra -g -Wpedantic -Wstrict-prototypes -Iinclude src/parser.c src/hackAssembler.c src/helper.c src/code.c -o build/hackAssembler -lm
