TARGET = dz_shell
TEST_TARGET = test_helpers
OBJ = main.c parser.c my_helpers.c builtins.c
TEST_OBJ = test_my_helpers.c my_helpers.c
CFLAGS = -Wall -Wextra -Werror
CC = gcc

all:
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