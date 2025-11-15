#ifndef MY_HELPERS_H
#define MY_HELPERS_H

#include <stddef.h>

int my_strcmp(const char *str1, const char *str2);
int my_strlen(const char *str);
int my_strncmp(const char *str1, const char *str2, size_t n);

char *my_getenv(char *variable_name, char **env);
char *my_strdup(const char *str);
char *my_strcpy(char *dest, const char *src);
char *my_strchr(const char *str, char c);
char *my_strtok(char *input, const char *delim);

#endif