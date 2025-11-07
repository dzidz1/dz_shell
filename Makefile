TARGET = dz_shell
OBJ = main.c parser.c my_helpers.c
CFLAGS = -Wall -Wextra -Werror
CC = gcc

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
clean:
	rm -f *-o 
fclean: clean
	rm -f $(TARGET)
re: fclean all