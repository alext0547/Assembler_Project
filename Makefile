# Makefile for Assembler + Compiler Project

# Compiler + tools
CC = gcc
BISON = bison
FLEX = flex

# Flags
CFLAGS = -std=c11 -O2 -Wall -Wextra -g -MMD -MP
LDFLAGS =
LDLIBS = -lm -lfl

# Source files
BISON_SRC = assembler.y
FLEX_SRC = assembler.l

# Generated files
PARSER_C = assembler.tab.c
PARSER_H = assembler.tab.h
LEXER_C = lex.yy.c

# Project sources
SRCS = main.c pass.c pass1.c pass2.c ir.c symtab.c $(PARSER_C) $(LEXER_C)
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

# Targets
EXEC = assembler
all: $(EXEC)

# Bison: parser
$(PARSER_C) $(PARSER_H): $(BISON_SRC)
	$(BISON) -Wall -d -o $(PARSER_C) $(BISON_SRC)

# Flex: lexer
$(LEXER_C): $(FLEX_SRC) $(PARSER_H)
	$(FLEX) -o $@ $<

# Compile .c -> .o
CFLAGS += -D_POSIX_C_SOURCE=200809L
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Run the assembler
RUN_ARGS ?=
.PHONY: run
run: $(EXEC)
	./$(EXEC) $(RUN_ARGS)

# Clean up generated files
clean:
	rm -f $(EXEC) $(OBJS) $(DEPS) $(PARSER_C) $(PARSER_H) $(LEXER_C)

.PHONY: all clean
-include $(DEPS)