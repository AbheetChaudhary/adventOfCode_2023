#include "lib/aoclib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 512
#define MAX_GAMES 128
#define TOKEN_BUF_SIZE 8
#define RED 12
#define GREEN 13
#define BLUE 14
#define MAX_TOKENS 128

struct Game {
  int game_id;
  char **tokens;
};

int min_cubes(struct Game *, const char *, int);
int red_check(struct Game *, int);
int green_check(struct Game *, int);
int blue_check(struct Game *, int);

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: %s input_file\n", argv[0]);
    return 0;
  }

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("Could not open file: %s\n", argv[1]);
    return 0;
  }

  const int tokenBufSize = TOKEN_BUF_SIZE;
  const int maxTokens = MAX_TOKENS;

  char line[LINE_LEN];
  int idSum = 0;
  int powerSum = 0;

  while (fgets(line, LINE_LEN, fp)) {
    struct Game *game = (struct Game *)malloc(sizeof(struct Game));

    // get game id
    FILE *lp_gameId = fmemopen((void *)line, strstr(line, ":") - line, "r");
    char *token = get_next_token(lp_gameId, tokenBufSize);
    token = get_next_token(lp_gameId, tokenBufSize);
    game->game_id = atoi(token);

    FILE *lp = fmemopen((void *)line, strlen(line), "r");
    char **tokens = (char **)malloc(maxTokens * sizeof(char *));
    int token_count = alnum_parser(lp, tokens, maxTokens, tokenBufSize);
    game->tokens = tokens;

    powerSum += (min_cubes(game, "red", token_count) *
                 min_cubes(game, "green", token_count) *
                 min_cubes(game, "blue", token_count));

    if (!red_check(game, token_count) || !green_check(game, token_count) ||
        !blue_check(game, token_count))
      continue;

    idSum += game->game_id;
  }

  printf("ID sum of valid games: %d\n", idSum);
  printf("Power sum across all games: %d\n", powerSum);

  return 0;
}

int min_cubes(struct Game *game, const char *color, int token_count) {
  int minCubeCount = 0, cubeCount;
  for (int i = 0; i < token_count; i++) {
    if (strcmp(game->tokens[i], color) == 0) {
      if ((cubeCount = atoi(game->tokens[i - 1])) >= minCubeCount)
        minCubeCount = cubeCount;
    }
  }

  return minCubeCount;
}

int red_check(struct Game *game, int token_count) {
  int valid = 1;
  for (int i = 0; i < token_count; i++) {
    if (strcmp(game->tokens[i], "red") == 0) {
      if (atoi(game->tokens[i - 1]) > RED)
        valid = 0;
    }
  }

  return valid;
}

int green_check(struct Game *game, int token_count) {
  int valid = 1;
  for (int i = 0; i < token_count; i++) {
    if (strcmp(game->tokens[i], "green") == 0) {
      if (atoi(game->tokens[i - 1]) > GREEN)
        valid = 0;
    }
  }

  return valid;
}

int blue_check(struct Game *game, int token_count) {
  int valid = 1;
  for (int i = 0; i < token_count; i++) {
    if (strcmp(game->tokens[i], "blue") == 0) {
      if (atoi(game->tokens[i - 1]) > BLUE)
        valid = 0;
    }
  }

  return valid;
}
