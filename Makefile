TARGET = dz_shell
TEST_TARGET = test_helpers
TEST_PARSER = test_parser
TEST_BUILTINS = test_builtins
SRC_DIR = src
INC_DIR = include
TEST_DIR = tests

OBJ = $(SRC_DIR)/main.c $(SRC_DIR)/parser.c $(SRC_DIR)/my_helpers.c $(SRC_DIR)/builtins.c $(SRC_DIR)/executor.c
TEST_OBJ = $(TEST_DIR)/test_my_helpers.c $(SRC_DIR)/my_helpers.c
TEST_PARSER_OBJ = $(TEST_DIR)/test_parser.c $(SRC_DIR)/parser.c $(SRC_DIR)/my_helpers.c
TEST_BUILTINS_OBJ = $(TEST_DIR)/test_builtins.c $(SRC_DIR)/builtins.c $(SRC_DIR)/my_helpers.c

CFLAGS = -Wall -Wextra -Werror -I$(INC_DIR)
CC = gcc

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

test: $(TEST_TARGET) $(TEST_PARSER) $(TEST_BUILTINS)
	./$(TEST_TARGET)
	./$(TEST_PARSER)
	./$(TEST_BUILTINS)

$(TEST_TARGET):
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJ)

$(TEST_PARSER):
	$(CC) $(CFLAGS) -o $(TEST_PARSER) $(TEST_PARSER_OBJ)

$(TEST_BUILTINS):
	$(CC) $(CFLAGS) -o $(TEST_BUILTINS) $(TEST_BUILTINS_OBJ)

test_parser_only: $(TEST_PARSER)
	./$(TEST_PARSER)

test_helpers_only: $(TEST_TARGET)
	./$(TEST_TARGET)

test_builtins_only: $(TEST_BUILTINS)
	./$(TEST_BUILTINS)

clean:
	rm -f *.o
	
fclean: clean
	rm -f $(TARGET) $(TEST_TARGET) $(TEST_PARSER) $(TEST_BUILTINS)
	
re: fclean all

.PHONY: all test test_parser_only test_helpers_only clean fclean re