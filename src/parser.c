#include "parser.h"
#include "common.h"

char **parse_input(char *input) {
  size_t buffer_size = MAX_INPUT;
  char **tokens = malloc(buffer_size * sizeof(char *));
  if (!tokens) {
    perror("Malloc failed");
    exit(EXIT_FAILURE);
  }

  char *token = NULL;
  size_t token_length = 0;
  size_t position = 0;
  size_t i = 0;

  while (input[i] != '\0') {

    // skip leading whitespace characters
    while (input[i] != '\0' &&
           (input[i] == ' ' || input[i] == '\n' || input[i] == '\t' ||
            input[i] == '\r' || input[i] == '\a')) {
      i++;
    }

    if (input[i] == '\0')
      break;

    token = &input[i];

    // calculating token length
    while (input[i] != '\0' && input[i] != ' ' && input[i] != '\n' &&
           input[i] != '\t' && input[i] != '\r' && input[i] != '\a') {
      token_length++;
      i++;
    }

    tokens[position] = malloc((token_length + 1) * sizeof(char));
    if (!tokens[position]) {
      perror("Malloc failed");
      exit(EXIT_FAILURE);
    }

    // copying token at address tokens[position]
    for (size_t j = 0; j < token_length; j++) {
      tokens[position][j] = token[j];
    }
    tokens[position][token_length] = '\0'; // null terminate

    position++;
    token_length = 0;
  }

  if (position == 0) {
    free(tokens);
    return NULL;
  }

  tokens[position] = NULL; // terminating array with null

  return tokens;
}

// free allocated tokens memory
void free_tokens(char **tokens) {
  if (!tokens)
    return;

  for (size_t i = 0; tokens[i]; i++) {
    free(tokens[i]); // each token
  }

  free(tokens); // array
}