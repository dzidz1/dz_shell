#ifndef BUILTINS_H
#define BUILTINS_H

int shell_builts(char **args, char **env, char *initial_directory);

int command_cd(char **args, char *initial_directory, char **env);
int command_pwd();
int command_echo(char **args, char **env);
int command_env(char **env);
int command_which(char **args, char **env);

char **command_setenv(char **args, char **env);
char **command_unsetenv(char **args, char **env);

#endif
