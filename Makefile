CC = gcc

# FLAGS
LDFLAGS = -lcriterion
CFLAGS = -Wall -I. -Iinclude

# FOLDERS
SRC_DIR=src
TESTS_DIR=tests
DIST_DIR=dist

# FILES
MAIN_TARGET = $(DIST_DIR)/main
OFLEX_TARGET = $(DIST_DIR)/oflex
TEST = $(wildcard tests/*.c)
SRC = $(wildcard $(SRC_DIR)/*.c) 
SRC_OBJS = $(SRC:$(SRC_DIR)/%.c=$(DIST_DIR)/%.o)

# TESTOBJ = $(TEST:.c=.o)

all: $(DIST_DIR) $(MAIN_TARGET) $(OFLEX_TARGET)

$(DIST_DIR):
	mkdir $(DIST_DIR)

$(MAIN_TARGET): $(SRC_OBJS)
	$(CC) $(CFLAGS) -o $(MAIN_TARGET) $(DIST_DIR)/main.o $(DIST_DIR)/file_buffer.o $(DIST_DIR)/lexer.o $(DIST_DIR)/lexer_table.o $(DIST_DIR)/utils.o

$(OFLEX_TARGET): $(SRC_OBJS) 
	$(CC) $(CFLAGS) -o $(OFLEX_TARGET) $(DIST_DIR)/oflex.o $(DIST_DIR)/file_buffer.o $(DIST_DIR)/lexer.o

$(DIST_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

oflex: $(OFLEX_TARGET)
	./$(OFLEX_TARGET) table/table.csv

run: $(MAIN_TARGET)
	./$(MAIN_TARGET) input/input.txt

val: $(MAIN_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(MAIN_TARGET)

clean:
	rm $(DIST_DIR) -R
	rm $(SRC_DIR)/lexer_table.c

.PHONY: all clean
