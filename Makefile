# Makefile for Assembler + Compiler Project

# Compiler + tools
CC = gcc
BISON = bison
FLEX = flex

# Dirs
SRCDIR = Assembler/src
INCDIR = Assembler/include
TEST_DIR := Assembler/testing

# Flags
CPPFLAGS = -I$(INCDIR)
CFLAGS = -std=c11 -O2 -Wall -Wextra -g -MMD -MP -D_POSIX_C_SOURCE=200809L -D_FILE_OFFSET_BITS=64
LDLIBS = -lm -lfl

# Source files
BISON_SRC = $(SRCDIR)/assembler.y
FLEX_SRC = $(SRCDIR)/assembler.l

# Generated files
PARSER_C = $(SRCDIR)/assembler.tab.c
PARSER_H = $(SRCDIR)/assembler.tab.h
LEXER_C = $(SRCDIR)/lex.yy.c

# Project sources
SRCS = \
$(SRCDIR)/main.c \
$(SRCDIR)/pass.c \
$(SRCDIR)/pass1.c \
$(SRCDIR)/pass2.c \
$(SRCDIR)/ir.c \
$(SRCDIR)/symtab.c \
$(SRCDIR)/c_rules.c \
$(SRCDIR)/out.c \
$(PARSER_C) \
$(LEXER_C)

OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

# Targets
EXEC = assembler
all: $(EXEC)

# Bison: parser
$(PARSER_C) $(PARSER_H): $(SRCDIR)/assembler.y
	$(BISON) -Wall -d -o $(PARSER_C) $<

# Flex: lexer
$(LEXER_C): $(SRCDIR)/assembler.l $(PARSER_H)
	$(FLEX) -o $(LEXER_C) $<

# Compile .c -> .o
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Link
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

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

.PHONY: test
test: $(EXEC)
	@./run_all_tests.sh $(TEST_DIR)

.PHONY: test-valgrind
test-valgrind:
	@echo "Running tests under Valgrind (this may take longer)..."
	@chmod +x run_all_tests.sh
	VALGRIND=1 ./run_all_tests.sh
