#include "dz_shell.h"

// cd, cd [path], cd - (prev dir), cd ~, cd .., handling non existing dirs and
// permission issues
int command_cd(char **args, char *initial_directory) {
  (void)initial_directory; // not needed yet
  if (args[1] == NULL) {
    printf("cd expects path as an argument\n");
  } else if (chdir(args[1]) == 0) {
    printf("CD worked\n");
  } else {
    perror("CD failed");
  }
  return 0;
}

int command_pwd() {
  char *cwd = getcwd(NULL, 0);

  if (cwd == NULL) {
    perror("PWD failed");
  } else {
    printf("%s\n", cwd);
  }
  free(cwd);

  return 0;
}

// echo hello, echo $PATH, echo -n hello
int command_echo(char **args, char **env) {
  (void)env;
  if (args[1] == NULL) {
    printf("\n");
  } else {
    char no_newline = 0;
    size_t i = 1;
    if (my_strcmp(args[1], "-n") == 0) {
      no_newline = 1;
      i = 2;
    }

    for (; args[i] != NULL; i++) {
      if (args[i][0] == '$') {
        char *variable_value = my_getenv(&args[i][1], env);
        if (variable_value) {
          printf("%s", variable_value);
        }
      } else {
        printf("%s", args[i]);
      }

      if (args[i + 1] != NULL) {
        printf(" ");
      }
    }

    if (no_newline == 0) {
      printf("\n");
    }
  }

  return 0;
}

int command_env(char **env) {
  for (size_t i = 0; env[i] != 0; i++) {
    printf("%s\n", env[i]);
  }

  return 0;
}

char *find_command_in_path(const char *command, char **env) {
  char *path_env = NULL; // PATH value
  char *path = NULL;     // duplicate PATH
  char *token = NULL;    // tokenizing dirs in PATH
  char full_path[1024];  // buffer to construct full path

  // PATH value location
  for (size_t i = 0; env[i]; i++) {
    if (my_strncmp(env[i], "PATH=", 5) == 0) {
      path_env = env[i] + 5; // skipping prefix
      break;
    }
  }

  if (!path_env)
    return NULL;

  path = my_strdup(path_env);

  // splitting PATH into particular directories using ':' as a delimiter
  token = my_strtok(path, ":");
  while (token != NULL) {
    // Construct full path
    int len = my_strlen(token);
    if (token[len - 1] == '/') {
      snprintf(full_path, sizeof(full_path), "%s%s", token, command);
    } else {
      snprintf(full_path, sizeof(full_path), "%s%s%s", token, "/", command);
    }

    // checking if the command exists and is executable
    if (access(full_path, X_OK) == 0) {
      free(path);
      return my_strdup(full_path); // found commands path
    }

    token = my_strtok(NULL, ":"); // next token
  }

  free(path);
  return NULL;
}

int command_which(char **args, char **env) {
  if (args[1] == NULL) {
    printf("which command expects an argument\n");
    return 1;
  }

  // our builtins list
  const char *builtins[] = {"cd",       "pwd",   "echo", "env", "setenv",
                            "unsetenv", "which", "exit", NULL};
  for (size_t i = 0; builtins[i] != NULL; i++) {
    if (my_strcmp(args[1], builtins[i]) == 0) {
      printf("%s is a shell builtin command\n", args[i]);
      return 0;
    }
  }

  // other commands
  char *full_path = find_command_in_path(args[1], env);
  if (full_path != NULL) {
    printf("%s\n", full_path);
    free(full_path);
    return 0;
  }

  printf("which %s command not found\n", args[1]);
  return 1;
}

char **command_setenv(char **args, char **env);
char **command_unsetenv(char **args, char **env);