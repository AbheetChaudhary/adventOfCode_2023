#ifndef AOCLIB_H
#define AOCLIB_H

#include <stdio.h>
void super_strstr(char *haystack, char *needle, int *indices);

char *get_next_token(FILE *lp, const int tokenBufSize);

int alnum_parser(FILE *lp, char **tokens, const int maxTokens,
                 const int tokenBufSize);

#endif
