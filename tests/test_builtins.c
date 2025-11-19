#include "builtins.h"
#include "common.h"
#include "my_helpers.h"

// Generic stdout capture with cleanup helper
typedef struct {
  int pipe_read;
  int stdout_backup;
} capture_ctx_t;

static capture_ctx_t capture_stdout_begin(void) {
  capture_ctx_t ctx;
  int pipefd[2];

  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  ctx.stdout_backup = dup(fileno(stdout));
  if (ctx.stdout_backup == -1) {
    perror("dup");
    exit(EXIT_FAILURE);
  }

  if (dup2(pipefd[1], fileno(stdout)) == -1) {
    perror("dup2");
    exit(EXIT_FAILURE);
  }
  close(pipefd[1]);

  ctx.pipe_read = pipefd[0];
  return ctx;
}

static char *capture_stdout_end(capture_ctx_t ctx) {
  fflush(stdout);

  if (dup2(ctx.stdout_backup, fileno(stdout)) == -1) {
    perror("dup2 restore");
    exit(EXIT_FAILURE);
  }
  close(ctx.stdout_backup);

  char buffer[4096];
  ssize_t n = read(ctx.pipe_read, buffer, sizeof(buffer) - 1);
  if (n < 0) {
    perror("read");
    exit(EXIT_FAILURE);
  }
  close(ctx.pipe_read);

  buffer[n] = '\0';
  return my_strdup(buffer);
}

// Simplified capture functions using the generic helper
char *capture_stdout_for_echo(char **args, char **env) {
  capture_ctx_t ctx = capture_stdout_begin();
  command_echo(args, env);
  return capture_stdout_end(ctx);
}

char *capture_stdout_void(void (*fn)(void)) {
  capture_ctx_t ctx = capture_stdout_begin();
  fn();
  return capture_stdout_end(ctx);
}

char *capture_stdout_for_env(char **env) {
  capture_ctx_t ctx = capture_stdout_begin();
  command_env(env);
  return capture_stdout_end(ctx);
}

char *capture_stdout_for_which(char **args, char **env) {
  capture_ctx_t ctx = capture_stdout_begin();
  command_which(args, env);
  return capture_stdout_end(ctx);
}

// Helper for test assertions
static void assert_str_eq(const char *expected, const char *actual,
                          const char *test_name, int *passed) {
  if (my_strcmp(actual, expected) == 0) {
    printf("%s passed\n", test_name);
    (*passed)++;
  } else {
    printf("%s FAILED\nExpected: '%s'\nGot:      '%s'\n", test_name, expected,
           actual);
  }
}

static void strip_trailing_newline(char *str) {
  int len = my_strlen(str);
  if (len > 0 && str[len - 1] == '\n') {
    str[len - 1] = '\0';
  }
}

// Test functions
int test_command_echo(char **env) {
  int total = 0, passed = 0;

  // Test 1: echo hello world
  total++;
  {
    char *args[] = {"echo", "hello", "world", NULL};
    char *out = capture_stdout_for_echo(args, env);
    assert_str_eq("hello world\n", out, "Test 1: echo hello world", &passed);
    free(out);
  }

  // Test 2: echo -n hello
  total++;
  {
    char *args[] = {"echo", "-n", "hello", NULL};
    char *out = capture_stdout_for_echo(args, env);
    assert_str_eq("hello", out, "Test 2: echo -n hello", &passed);
    free(out);
  }

  // Test 3: echo $PATH
  total++;
  {
    char *args[] = {"echo", "$PATH", NULL};
    char *out = capture_stdout_for_echo(args, env);
    int len = my_strlen(out);
    if (len > 0 && out[len - 1] == '\n') {
      printf("Test 3 passed: echo $PATH (ends with newline)\n");
      passed++;
    } else {
      printf("Test 3 FAILED: echo $PATH (no newline at the end), got: '%s'\n",
             out);
    }
    free(out);
  }

  printf("\ncommand_echo: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

static void pwd_wrapper(void) { command_pwd(); }

int test_command_pwd(void) {
  int total = 0, passed = 0;
  char *original_cwd = getcwd(NULL, 0);
  if (!original_cwd) {
    perror("getcwd");
    return 0;
  }

  // Test 1: pwd prints current directory
  total++;
  {
    char *expected = getcwd(NULL, 0);
    char *output = capture_stdout_void(pwd_wrapper);
    strip_trailing_newline(output);

    if (expected && my_strcmp(expected, output) == 0) {
      printf("Test 1 passed: pwd prints current directory\n");
      passed++;
    } else {
      printf("Test 1 FAILED: pwd\nExpected: '%s'\nGot:     '%s'\n",
             expected ? expected : "(null)", output ? output : "(null)");
    }
    free(expected);
    free(output);
  }

  // Test 2: pwd after chdir to /
  total++;
  {
    if (chdir("/") == 0) {
      char *output = capture_stdout_void(pwd_wrapper);
      strip_trailing_newline(output);
      assert_str_eq("/", output, "Test 2: pwd after chdir to '/'", &passed);
      free(output);
    } else {
      perror("chdir to /");
      printf("Test 2 FAILED: could not chdir to '/'\n");
    }
  }

  chdir(original_cwd);
  free(original_cwd);

  printf("\ncommand_pwd: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_command_env(void) {
  int total = 0, passed = 0;
  char *fake_env[] = {"PATH=/usr/bin", "HOME=/home/ako", "USER=ako", NULL};

  total++;
  {
    char *out = capture_stdout_for_env(fake_env);
    const char *expected = "PATH=/usr/bin\nHOME=/home/ako\nUSER=ako\n";
    assert_str_eq(expected, out, "Test 1: env prints all environment variables",
                  &passed);
    free(out);
  }

  printf("\ncommand_env: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_command_which(char **env) {
  int total = 0, passed = 0;

  // Test 1: builtin cd
  total++;
  {
    char *args[] = {"which", "cd", NULL};
    char *out = capture_stdout_for_which(args, env);
    assert_str_eq("cd is a shell builtin command\n", out,
                  "Test 1: which cd (builtin)", &passed);
    free(out);
  }

  // Test 2: missing argument
  total++;
  {
    char *args[] = {"which", NULL};
    char *out = capture_stdout_for_which(args, env);
    assert_str_eq("which command expects an argument\n", out,
                  "Test 2: which with no argument", &passed);
    free(out);
  }

  // Test 3: non-existent command
  total++;
  {
    char *args[] = {"which", "definitely_not_existing_12345", NULL};
    char *out = capture_stdout_for_which(args, env);
    assert_str_eq("which definitely_not_existing_12345 command not found\n",
                  out, "Test 3: which non-existent command", &passed);
    free(out);
  }

  // Test 4: external command like 'ls'
  total++;
  {
    char *args[] = {"which", "ls", NULL};
    char *out = capture_stdout_for_which(args, env);
    if (out[0] == '/') {
      printf("Test 4 passed: which ls (path starts with '/')\n");
      passed++;
    } else {
      printf("Test 4 FAILED: which ls, got: '%s'\n", out);
    }
    free(out);
  }

  printf("\ncommand_which: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_command_cd(void) {
  int total = 0, passed = 0;
  char *original_cwd = getcwd(NULL, 0);
  if (!original_cwd) {
    perror("getcwd");
    return 0;
  }

  // Test 1: cd /
  total++;
  {
    char *args[] = {"cd", "/", NULL};
    command_cd(args, NULL);
    char *cwd = getcwd(NULL, 0);
    if (cwd && my_strcmp(cwd, "/") == 0) {
      printf("Test 1 passed: cd /\n");
      passed++;
    } else {
      printf("Test 1 FAILED: cd /, cwd = '%s'\n", cwd ? cwd : "(null)");
    }
    free(cwd);
  }

  // Test 2: cd to non-existent directory
  total++;
  {
    char *before = getcwd(NULL, 0);
    char *args[] = {"cd", "/definitely/not/here_12345", NULL};
    command_cd(args, NULL);
    char *after = getcwd(NULL, 0);

    if (before && after && my_strcmp(before, after) == 0) {
      printf("Test 2 passed: cd invalid path does not change cwd\n");
      passed++;
    } else {
      printf("Test 2 FAILED: cd invalid path changed cwd\nBefore: '%s'\nAfter: "
             " '%s'\n",
             before ? before : "(null)", after ? after : "(null)");
    }
    free(before);
    free(after);
  }

  chdir(original_cwd);
  free(original_cwd);

  printf("\ncommand_cd: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_command_setenv() {
  int total = 0;
  int passed = 0;

  // Test 1: args[1] == NULL
  total++;
  {
    char *args[] = {"setenv", NULL, NULL};
    char *env[] = {"x=5", "y=10", "z=15", "k=20", NULL};
    char **res = command_setenv(args, env);
    if (res == env) {
      printf("Test 1 passed: args[1] is NULL\n");
      passed++;
    } else {
      printf(
          "Test 1 FAILED: args[1] is NULL, res expected to be same as env\n");
    }
  }

  // Test 2: doesnt contain '='
  total++;
  {
    char *args[] = {"setenv", "VARvalue", NULL};
    char *env[] = {"x=5", "y=10", "z=15", "k=20", NULL};
    char **res = command_setenv(args, env);
    if (res == env) {
      printf("Test 2 passed: args[1] doesnt contain '='\n");
      passed++;
    } else {
      printf("Test 2 FAILED: args[1] doesnt contain '=', res expected to be "
             "the same as env\n");
    }
  }

  // Test 3: adding new var
  total++;
  {
    char *args[] = {"setenv", "VAR=value", NULL};
    char *env[] = {"x=5", "y=10", "z=15", "k=20", NULL};
    char **res = command_setenv(args, env);
    if (my_strcmp(res[4], "VAR=value") == 0) {
      printf("Test 3 passed: adding new var\n");
      passed++;
      free(res);
    } else {
      printf("Test 3 FAILED: adding new var, last var expected VAR=value\n");
    }
  }

  // Test 4: reset existing var
  total++;
  {
    char *args[] = {"setenv", "y=25", NULL};
    char *env[] = {"x=5", "y=10", "z=15", "k=20", NULL};
    char **res = command_setenv(args, env);
    if (my_strcmp(res[1], "y=25") == 0) {
      printf("Test 4 passed: reset existing var\n");
      passed++;
      free(res);
    } else {
      printf("Test 4 FAILED: reset existing var, expected y=25, got: %s\n",
             res[1]);
    }
  }

  // Test 5: empty value (var=)
  total++;
  {
    char *args[] = {"setenv", "var=", NULL};
    char *env[] = {"x=5", "y=10", "z=15", "k=20", NULL};
    char **res = command_setenv(args, env);
    if (my_strcmp(res[4], "var=") == 0) {
      printf("Test 5 passed: empty value\n");
      passed++;
      free(res);
    } else {
      printf("Test 5 FAILED: empty value, var expected to be empty\n");
    }
  }

  // Test 6: empty variable (=value)
  total++;
  {
    char *args[] = {"setenv", "=value", NULL};
    char *env[] = {"x=5", "y=10", "z=15", "k=20", NULL};
    char **res = command_setenv(args, env);
    if (res == env) {
      printf("Test 6 passed: empty variable\n");
      passed++;
    } else {
      printf("Test 6 FAILED: empty variable, res expected to be same as env\n");
    }
  }

  printf("\ncommand_setenv: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int main(int argc, char **argv, char **env) {
  (void)argc;
  (void)argv;

  int passed = 0, total = 0;
  printf("Running tests...\n\n");

  total++;
  passed += test_command_echo(env);
  printf("\n=========================\n");

  total++;
  passed += test_command_pwd();
  printf("\n=========================\n");

  total++;
  passed += test_command_env();
  printf("\n=========================\n");

  total++;
  passed += test_command_which(env);
  printf("\n=========================\n");

  total++;
  passed += test_command_cd();
  printf("\n=========================\n");

  total++;
  passed += test_command_setenv();
  printf("\n=========================\n");

  printf("Overall Builtins: %d/%d test functions passed\n", passed, total);
  return (passed == total) ? 0 : 1;
}