CC = gcc

# FLAGS
LDFLAGS = -lcriterion
CFLAGS = -Wall -I. -Iinclude

# FOLDERS
SRC_DIR=src
TESTS_DIR=tests
DIST_DIR=dist

# FILES
TARGET = $(DIST_DIR)/main
TEST = $(wildcard tests/*.c)
SRC = $(wildcard $(SRC_DIR)/*.c) 
SRC_OBJS = $(SRC:$(SRC_DIR)/%.c=$(DIST_DIR)/%.o)

# TESTOBJ = $(TEST:.c=.o)

all: $(DIST_DIR) $(TARGET) 

$(DIST_DIR):
	mkdir $(DIST_DIR)

$(TARGET): $(SRC_OBJS) 
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_OBJS)

$(DIST_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) input/tiny.txt

val: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

clean:
	rm $(DIST_DIR) -R

.PHONY: all clean
