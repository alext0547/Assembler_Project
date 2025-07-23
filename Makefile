# Makefile for Assembler + Compiler Project

# Compiler + tools
CC = gcc
BISON = bison
FLEX = flex

# Flags
CFLAGS = -Wall -g
LDFLAGS = -lfl

# Source files
BISON_SRC = parser.y
FLEX_SRC = lexer.l
SRC = main.c parser.tab.c lex.yy.c

# Targets
EXEC = assembler
all: $(EXEC)

# Generate parser files
parser.tab.c parser.tab.h: $(BISON_SRC)
	$(BISON) -d $(BISON_SRC)

# Generate lexer file
lex.yy.c: $(FLEX_SRC)
	$(FLEX) $(FLEX_SRC)

# Compile executable
$(EXEC): parser.tab.c lex.yy.c main.c
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) $(LDFLAGS)

# Clean up generated files
clean:
	rm -f $(EXEC) parser.tab.c parser.tab.h lex.yy.c *.o

.PHONY: all clean