TARGET = dz_shell
TEST_TARGET = test_helpers
SRC_DIR = src
INC_DIR = include
TEST_DIR = tests

OBJ = $(SRC_DIR)/main.c $(SRC_DIR)/parser.c $(SRC_DIR)/my_helpers.c $(SRC_DIR)/builtins.c
TEST_OBJ = $(TEST_DIR)/test_my_helpers.c $(SRC_DIR)/my_helpers.c

CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR)
CC = gcc

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET):
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJ)

clean:
	rm -f *.o
	
fclean: clean
	rm -f $(TARGET) $(TEST_TARGET)
	
re: fclean all

.PHONY: all test clean fclean re