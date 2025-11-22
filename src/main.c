#include "builtins.h"
#include "common.h"
#include "executor.h"
#include "my_helpers.h"
#include "parser.h"

// shell loop
// input parsing
// builtin commands execution
// execute external commands
// manage environmental variables
// manage path
// error handling

int shell_builts(char **args, char **env, char *initial_directory) {
  // dont need initial_directory yet
  (void)initial_directory;
  if (my_strcmp(args[0], "cd") == 0) {
    return command_cd(args, initial_directory, env);
  } else if (my_strcmp(args[0], "pwd") == 0) {
    return command_pwd();
  } else if (my_strcmp(args[0], "echo") == 0) {
    return command_echo(args, env);
  } else if (my_strcmp(args[0], "env") == 0) {
    return command_env(env);
  } else if (my_strcmp(args[0], "which") == 0) {
    return command_which(args, env);
  } else if (my_strcmp(args[0], "exit") == 0 ||
             my_strcmp(args[0], "quit") == 0) {
    exit(EXIT_SUCCESS);
  } else {
    return executor(args, env);
  }

  return 0;
}

void shell_loop(char **env) {
  char *input = NULL;
  size_t input_size = 0;

  char **args; // input arguments array

  char *initial_directory = getcwd(NULL, 0); // get current working directory

  while (1) {
    printf("<dz_shell> ");

    if (getline(&input, &input_size, stdin) == -1) {
      perror("getline");
      break; // EOF
    }

    args = parse_input(input);

    if (args == NULL) {
      continue;
    }

    if (my_strcmp(args[0], "setenv") == 0) {
      env = command_setenv(args, env);
    } else if (my_strcmp(args[0], "unsetenv") == 0) {
      env = command_unsetenv(args, env);
    } else {
      shell_builts(args, env, initial_directory);
    }

    free_tokens(args);
  }

  free(input);
  free(initial_directory);
}

int main(int argc, char **argv, char **env) {
  // dont need argc and argv
  (void)argc;
  (void)argv;

  shell_loop(env);

  return 0;
}