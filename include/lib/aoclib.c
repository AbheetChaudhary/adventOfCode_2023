#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "aoclib.h"

void super_strstr(char *haystack, char *needle, int *indices) {
  char *ptr, *temp_haystack;
  temp_haystack = haystack;
  int i;
  for(i = 0; (ptr = strstr(haystack, needle)) != NULL; i++) {
    indices[i] = ptr - temp_haystack;
    haystack = ptr + 1;
  }

  indices[i] = -1;
}

char *get_next_token(FILE *lp, const int tokenBufSize) {
  char token[tokenBufSize];
  char *word = (char *)malloc(sizeof(token));
  int token_len = 0;
  int token_count = 0;
  int c, last_c = '\0';

  while (token_count < 1 && (c = fgetc(lp)) != EOF) {
    if (isalnum(c)) {
      if (token_len < tokenBufSize - 1) {
        token[token_len++] = c;
        token[token_len] = '\0';
        last_c = c;
      } else {
        fprintf(stderr,
                "token_len(%d) cannot be stored in tokenBufSize(%d) as a null "
                "terminated string\n",
                token_len, tokenBufSize);
        exit(-1);
      }
    } else if (isalnum(last_c)) {
      strcpy(word, token);
      token_count++;
      last_c = c;
    } else {
      last_c = c;
    }
  }

  if (isalnum(last_c) && token_count == 0) {
    strcpy(word, token);
    token_count++;
  }

  return word;
}

int alnum_parser(FILE *lp, char **tokens, const int maxTokens, const int tokenBufSize) {
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
        fprintf(stderr, "token_len(%d) cannot be stored in tokenBufSize(%d) as a null terminated string\n", token_len, tokenBufSize);
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
