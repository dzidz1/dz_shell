#include "common.h"
#include "my_helpers.h"
#include "parser.h"

int test_parse_input() {
  int passed = 0;
  int total = 0;

  // Test 1: normal command
  total++;
  char **tokens = parse_input("ls -la /home");
  if (tokens != NULL && my_strcmp(tokens[0], "ls") == 0 &&
      my_strcmp(tokens[1], "-la") == 0 && my_strcmp(tokens[2], "/home") == 0 &&
      tokens[3] == NULL) {
    printf("Test 1 passed: normal command\n");
    passed++;
  } else {
    printf("Test 1 FAILED: normal command parsing\n");
  }
  free_tokens(tokens);

  // Test 2: empty string
  total++;
  tokens = parse_input("");
  if (tokens == NULL) {
    printf("Test 2 passed: empty string\n");
    passed++;
  } else {
    printf("Test 2 FAILED: empty string, expected NULL\n");
  }

  // Test 3: only spaces
  total++;
  tokens = parse_input("  \t  ");
  if (tokens == NULL) {
    printf("Test 3 passed: only whitespaces\n");
    passed++;
  } else {
    printf("Test 3 FAILED: only whitespaces, expected NULL\n");
  }

  printf("\nparse_input: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int test_free_tokens() {
  int passed = 0;
  int total = 0;

  // Test 1: free NULL (should not crash :D)
  total++;
  free_tokens(NULL);
  printf("Test 1 passed: free NULL\n");
  passed++;

  // Test 2: free actual tokens
  total++;
  char **tokens = parse_input("test command");
  free_tokens(tokens);
  printf("Test 2 passed: free actual tokens\n");
  passed++;

  printf("\nfree_tokens: %d/%d tests passed\n", passed, total);
  return (passed == total) ? 1 : 0;
}

int main() {
  int passed = 0;
  int total = 0;

  printf("Running parser tests...\n\n");

  total++;
  passed += test_parse_input();
  printf("\n=========================\n");

  total++;
  passed += test_free_tokens();
  printf("\n=========================\n");

  printf("Overall Parser: %d/%d test functions passed\n", passed, total);
  return (passed == total) ? 0 : 1;
}