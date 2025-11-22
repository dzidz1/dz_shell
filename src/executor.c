#include "executor.h"
#include "common.h"
#include "my_helpers.h"

// Extracts and returns the PATH environment variable as a string
// Returns: duplicated PATH value (e.g., "/usr/bin:/bin:/usr/local/bin") or NULL
// if not found
char *get_path(char **env) {
  for (int i = 0; env[i]; i++) {
    if (my_strncmp(env[i], "PATH=", 5) == 0) {
      return my_strdup(env[i] + 5);
    }
  }

  return NULL;
}

// Splits the PATH string into an array of individual directory paths
// Parameters: paths - the PATH string to split, count - pointer to store the
// number of paths Returns: array of path strings (e.g., ["/usr/bin", "/bin",
// "/usr/local/bin"])
char **split_paths(char *paths, int *count) {
  if (!paths) {
    *count = 0;
    return NULL;
  }

  char **result = NULL;
  char *token;
  int size_of_path = my_strlen(paths);
  char paths_copy[size_of_path + 1];

  my_strncpy(paths_copy, paths, size_of_path + 1);
  paths_copy[size_of_path] = '\0';

  token = my_strtok(paths_copy, ":");
  *count = 0;

  while (token) {
    result = realloc(result, ((*count + 1) * sizeof(char *)));
    if (result == NULL) {
      perror("realloc failed");
      return NULL;
    }
    result[*count] = my_strdup(token);
    if (result[*count] == NULL) {
      perror("my_strdup failed");
      return NULL;
    }

    (*count)++;
    token = my_strtok(NULL, ":");
  }

  return result;
}

// Main executor: forks a child process to run the command, parent waits for
// completion Parameters: args - command and arguments array, env - environment
// variables Returns: 1 (used for shell continuation)
int executor(char **args, char **env) {
  pid_t pid;

  pid = fork();
  if (pid == -1) {
    perror("fork failed");
    return 1;
  }

  if (pid == 0 && child_process(args, env) == 1) {
    perror("execve failed");
    return 1;
  }

  int status;
  if (pid != 0 && waitpid(pid, &status, 0) == -1) {
    perror("waitpid failed");
    return 1;
  }

  if (pid != 0 && WIFSIGNALED(status)) {
    printf("process terminated by signal: %d\n", WTERMSIG(status));
  }

  return 1;
}

// Child process: searches for and executes the command
// Handles both direct paths (./program, /bin/ls) and PATH search (ls, cat)
// This function only returns if execve fails
int child_process(char **args, char **env) {
  int has_slash = 0;
  if (my_strchr(args[0], '/') != NULL)
    has_slash = 1;

  if (has_slash) {
    execve(args[0], args, env);
    perror("execve failed");
    return 1;
  }

  char *path_string = get_path(env);
  int num_paths;
  char **path_list = split_paths(path_string, &num_paths);

  for (int i = 0; i < num_paths; i++) {
    char full_path[MAX_INPUT];
    snprintf(full_path, sizeof(full_path), "%s/%s", path_list[i], args[0]);

    if (access(full_path, X_OK) == 0) {
      execve(full_path, args, env);
    }
  }

  for (int i = 0; i < num_paths; i++) {
    free(path_list[i]);
  }
  free(path_string);
  free(path_list);

  perror("command not found");
  return 1;
}
