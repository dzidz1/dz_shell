#ifndef DZ_SHELL_H
#define DZ_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_INPUT 1024

char **parse_input(char *input);
void free_tokens(char **tokens);

int shell_builts(char **args, char **env, char *initial_directory);

// built-ins
int command_cd(char **args, char *initial_directory);
int command_pwd();
int command_echo(char **args, char **env);
int command_env(char **env);
int command_which(char **args, char **env);

char **command_setenv(char **args, char **env);
char **command_unsetenv(char **args, char **env);

// my_helpers
int my_strcmp(const char *str1, const char *str2);
int my_strlen(const char *str);
int my_strncmp(const char *str1, const char *str2, size_t n);

char *my_getenv(char *variable_name, char **env);
char *my_strdup(const char *str);
char *my_strcpy(char *dest, const char *src);
char *my_strchr(const char *str, char c);
char *my_strtok(char *input, const char *delim);

#endif