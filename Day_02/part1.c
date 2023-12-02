#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

int alnumparser(FILE *, char **, const int, const int);
char *getnexttoken(FILE *, const int);
int min_cubes(struct Game *, const char *, int);
int redcheck(struct Game *, int);
int greencheck(struct Game *, int);
int bluecheck(struct Game *, int);

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("usage: %s input_file\n", argv[0]);
    return 0;
  }

  FILE *fp = fopen(argv[1], "r");
  if(fp == NULL) {
    printf("Could not open file: %s\n", argv[1]);
    return 0;
  }

  const int tokenBufSize = TOKEN_BUF_SIZE;
  const int maxTokens = MAX_TOKENS;

  struct Game *games[MAX_GAMES] = {NULL};
  char line[LINE_LEN];
  char buffer[tokenBufSize];
  int c;
  int idSum = 0;
  int powerSum = 0;


  while(fgets(line, LINE_LEN, fp)) {
    struct Game *game = (struct Game *) malloc(sizeof(struct Game));

    // get game id
    FILE *lp_gameId = fmemopen((void *) line, strstr(line, ":") - line, "r");
    char *token = getnexttoken(lp_gameId, tokenBufSize);
    token = getnexttoken(lp_gameId, tokenBufSize);
    game->game_id = atoi(token);

    FILE *lp = fmemopen((void *) line, strlen(line), "r");
    char **tokens = (char **) malloc(maxTokens * sizeof(char *));
    int token_count = alnumparser(lp, tokens, maxTokens, tokenBufSize);
    game->tokens = tokens;

    powerSum += (min_cubes(game, "red", token_count) * min_cubes(game, "green", token_count) * min_cubes(game, "blue", token_count));

    if(!redcheck(game, token_count) || !greencheck(game, token_count) || !bluecheck(game, token_count)) continue;

    idSum += game->game_id;
  }

  printf("ID sum of valid games: %d\n", idSum);
  printf("Power sum across all games: %d\n", powerSum);

  return 0;
}

int min_cubes(struct Game *game, const char *color, int token_count) {
  int minCubeCount = 0, cubeCount;
  for(int i = 0; i < token_count; i++) {
    if(strcmp(game->tokens[i], color) == 0) {
      if((cubeCount = atoi(game->tokens[i - 1])) >= minCubeCount) minCubeCount = cubeCount;
    }
  }

  return minCubeCount;
}

int redcheck(struct Game *game, int token_count) {
  int valid = 1;
  for(int i = 0; i < token_count; i++) {
    if(strcmp(game->tokens[i], "red") == 0) {
      if(atoi(game->tokens[i - 1]) > RED) valid = 0;
    }
  }

  return valid;
}

int greencheck(struct Game *game, int token_count) {
  int valid = 1;
  for(int i = 0; i < token_count; i++) {
    if(strcmp(game->tokens[i], "green") == 0) {
      if(atoi(game->tokens[i - 1]) > GREEN) valid = 0;
    }
  }

  return valid;
}

int bluecheck(struct Game *game, int token_count) {
  int valid = 1;
  for(int i = 0; i < token_count; i++) {
    if(strcmp(game->tokens[i], "blue") == 0) {
      if(atoi(game->tokens[i - 1]) > BLUE) valid = 0;
    }
  }

  return valid;
}

char *getnexttoken(FILE *lp, const int tokenBufSize) {
  char token[tokenBufSize];
  char *word = (char *) malloc(sizeof(token));
  int token_len = 0;
  int token_count = 0;
  int c, last_c = '\0';

  while(token_count < 1 && (c = fgetc(lp)) != EOF) {
     if(isalnum(c)) {
      if(token_len < tokenBufSize - 1) {
        token[token_len++] = c;
        token[token_len] = '\0';
        last_c = c;
      } else {
        fprintf(stderr, "token_len(%d) cannot be stored in tokenBufSize(%d) as a null terminated string\n", token_len, TOKEN_BUF_SIZE);
        exit(-1);
      }
     } else if(isalnum(last_c)) {
        strcpy(word, token);
        token_count++;
        last_c = c;
     } else {
       last_c = c;
     }
  }

  if(isalnum(last_c) && token_count == 0) {
    strcpy(word, token);
    token_count++;
  }

  return word;
}

int alnumparser(FILE *lp, char **tokens, const int maxTokens, const int tokenBufSize) {
  char token[tokenBufSize];
  int token_len = 0;
  int token_count = 0;
  int c, last_c = '\0';

  while((c = fgetc(lp)) != EOF) {
    if(isalnum(c)) {
      if(token_len < tokenBufSize - 1) {
        token[token_len++] = c;
        token[token_len] = '\0';
        last_c = c;
      } else {
        fprintf(stderr, "token_len(%d) cannot be stored in tokenBufSize(%d) as a null terminated string\n", token_len, TOKEN_BUF_SIZE);
        exit(-1);
      }
    } else if(isalnum(last_c)) {
      if(token_count < maxTokens) {
        char *word = (char *) malloc(sizeof(token));
        strcpy(word, token);
        token_len = 0;
        token[token_len] = '\0';
        tokens[token_count++] = word;
        last_c = c;
      } else {
        fprintf(stderr, "token limit reached. Cannot store more than maxTokens(%d)\n", maxTokens);
        exit(-1);
      }
    } else {
      last_c = c;
    }
  }

  if (isalnum(last_c)) {
    if(token_count < maxTokens) {
      char *word = (char *) malloc(sizeof(token));
      strcpy(word, token);
      token_len = 0;
      token[token_len] = '\0';
      tokens[token_count++] = word;
      last_c = c;
    } else {
      fprintf(stderr, "token limit reached. Cannot store more than maxTokens(%d)\n", maxTokens);
      exit(-1);
    }
  }

  return token_count;
}
