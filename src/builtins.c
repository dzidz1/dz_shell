#include "builtins.h"
#include "common.h"
#include "my_helpers.h"

// cd, cd [path], cd - (prev dir), cd ~, cd .., handling non existing dirs and
// permission issues
int command_cd(char **args, char *initial_directory, char **env) {
  char *path;

  // no argument or ~, goes to HOME
  if (args[1] == NULL || my_strcmp(args[1], "~") == 0) {
    path = my_getenv("HOME", env);
    if (path == NULL) {
      fprintf(stderr, "cd: HOME not set\n");
      return 1;
    }
  } else if (my_strcmp(args[1], "-") == 0) {
    path = initial_directory;
    if (path == NULL) {
      fprintf(stderr, "cd: initial directory not avaliable\n");
      return 1;
    }
  } else {
    path = args[1];
  }

  if (chdir(path) != 0) {
    perror("cd failed");
    return 1;
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
      printf("%s is a shell builtin command\n", builtins[i]);
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

int count_env_vars(char **env) {
  int res = 0;
  while (env[res]) {
    res++;
  }
  return res;
}

char **command_setenv(char **args, char **env) {
  if (args[1] == NULL) {
    printf("setenv gets const char *name and const char *value\nUsage: setenv "
           "VAR=value\n");
    return env;
  }

  // validating '='
  char *equal_sign = my_strchr(args[1], '=');
  if (equal_sign == NULL || equal_sign == args[1]) {
    printf("Error: invalid format. Use VAR=value\n");
    return env;
  }

  // extracting variable name
  char var_name[256];
  my_strncpy(var_name, args[1], equal_sign - args[1]);
  var_name[equal_sign - args[1]] = '\0';
  if (var_name[0] == '\0') {
    printf("Error: variale name can't be empty\n");
    return env;
  }

  int env_count = count_env_vars(env);
  char **new_env = malloc((env_count + 2) * sizeof(char *));
  if (new_env == NULL) {
    perror("malloc failed");
    return env;
  }

  int found = 0;
  int new_index = 0;

  for (int i = 0; i < env_count; i++) {
    // determine if we should set on this index
    char *existing_equal = my_strchr(env[i], '=');
    if (existing_equal != NULL) {
      size_t name_len = existing_equal - env[i];
      if (my_strncmp(env[i], var_name, name_len) == 0 &&
          var_name[name_len] == '\0') {
        // replace existing variable with new value
        new_env[new_index] = my_strdup(args[1]);
        found = 1;
      } else {
        // copy other vars
        new_env[new_index] = my_strdup(env[i]);
      }
    } else {
      new_env[new_index] = my_strdup(env[i]);
    }

    if (new_env[new_index] == NULL) {
      perror("strdup failed");
      for (int j = 0; j < new_index; j++) {
        free(new_env[j]);
      }
      free(new_env);
      return env;
    }

    new_index++;
  }

  if (!found) {
    new_env[new_index] = my_strdup(args[1]);
    if (new_env[new_index] == NULL) {
      perror("strdup failed");
      for (int j = 0; j < new_index; j++) {
        free(new_env[j]);
      }
      free(new_env);
      return env;
    }
    new_index++;
  }

  new_env[new_index] = NULL;
  return new_env;
}

char **command_unsetenv(char **args, char **env) {
  if (args[1] == NULL) {
    printf("unsetenv gets variable name as an argument\nUsage: unsetenv VAR\n");
    return env;
  }

  int vars_count = count_env_vars(env);

  char **new_env = malloc(vars_count * sizeof(char *));
  if (new_env == NULL) {
    perror("malloc failed");
    return env;
  }

  int j = 0;
  int found = 0;
  int len = my_strlen(args[1]);
  for (int i = 0; i < vars_count; i++) {
    if (found == 0 && my_strncmp(args[1], env[i], len) == 0 &&
        env[i][len] == '=') {
      found = 1;
    } else {
      new_env[j] = env[i];
      j++;
    }
  }

  if (found == 0) {
    printf("variable %s not found in env\n", args[1]);
    free(new_env);
    return env;
  }

  new_env[j] = NULL;
  return new_env;
}