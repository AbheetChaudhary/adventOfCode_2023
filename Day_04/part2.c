#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WINNING_NUMS 10 // maximum number of winning numbers
#define MAX_PLAYER_NUMS 25 // maximum number of numbers player can have
#define LINE_LEN 256
#define MAX_CARDS 256

struct Card {
  int timesWon;
  int score; // number of winning numbers the player has
};

/*
   Update each card in the cards array starting from the startFrom index.
   Updating a card means changing timesWon field of each card.
*/
void update_cards(struct Card **cards, int startFrom);

void print_cards(struct Card **cards, int numCards);

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: %s input_file\n", argv[0]);
    return 0;
  }

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "cannot open file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  char line[LINE_LEN];

  struct Card **cards = (struct Card **) malloc(MAX_CARDS * sizeof(struct Card *));
  int numCards = 0;  // number of cards

  int totalScore = 0;  // total score from all cards(original & duplicate)

  /*
     populate the 'cards' array to contain pointers to Card structs where ecah
     card is initialized with its points and timesWon as 1
  */
  while (fgets(line, LINE_LEN, fp) != NULL) {
    int winNums[MAX_WINNING_NUMS], playerNums[MAX_PLAYER_NUMS];
    int winNumCount = 0, playerNumCount = 0;
    int beginIndex = strstr(line, ":") - line;  // index of ':'

    // find the index of first number in the winning list
    while (!isdigit(line[beginIndex])) beginIndex++;

    // populate winning numbers array
    int number = 0;
    int i;
    int lastChar = 0;
    for (i = beginIndex; line[i] != '|'; i++) {
      if (isdigit(line[i])) {
        number = number * 10 + (line[i] - '0');
        lastChar = line[i];
      } else {
        if (isdigit(lastChar)) {
          winNums[winNumCount++] = number;
          number = 0;
          lastChar = line[i];
        } else {
          lastChar = line[i];
        }
      }
    }

    /* for (int index = 0; index < winNumCount; index++) { */
    /*   printf("%d, ", winNums[index]); */
    /* } */

    // find the index of first number in the players list
    while (!isdigit(line[i])) i++;

    // populate players numbers array
    for (i; i < strlen(line); i++) {
      if (isdigit(line[i])) {
        number = number * 10 + (line[i] - '0');
        lastChar = line[i];
      } else {
        if (isdigit(lastChar)) {
          playerNums[playerNumCount++] = number;
          number = 0;
          lastChar = line[i];
        } else {
          lastChar = line[i];
        }
      }
    }

    /* for (int index = 0; index < playerNumCount; index++) { */
    /*   printf("%d, ", playerNums[index]); */
    /* } */
    /* putchar('\n'); */

    int point = 0;
    for (int i = 0; i < playerNumCount; i++) {
      int exists = 0;
      for (int j = 0; j < winNumCount; j++) {
        if (winNums[j] == playerNums[i]) {
          exists = 1;
          break;
        }
      }

      if (exists) point++;
    }

    struct Card *card = (struct Card *) malloc(sizeof(struct Card));
    card->timesWon = 1;
    card->score = point;

    cards[numCards++] = card;
  }

  // update each card starting from the top
  for (int i = 0; i < numCards; i++) {
    update_cards(cards, i);
  }

  // accumulate all the occurances of each card in totalScore
  for (int i = 0; i < numCards; i++) {
    totalScore += cards[i]->timesWon;
  }

  printf("Total score from all cards: %d\n", totalScore);

  // free each card's memory then free the cards array
  for (int i = 0; i < numCards; i++) {
    free(cards[i]);
  }
  free(cards);

  return 0;

}

void print_cards(struct Card **cards, int numCards) {
  for (int i = 0; i < numCards; i++) {
    printf("{Card %d: timesWon = %d, score = %d}\n", i + 1, cards[i]->timesWon, cards[i]->score);
  }
}

void update_cards(struct Card **cards, int startFrom) {
  if (cards[startFrom]->score == 0) {
    return;
  } else {
    int cardsToUpdate = cards[startFrom]->score; // update this amount of next cards
    int thisCardCount = cards[startFrom]->timesWon;
    while (cardsToUpdate-- > 0) {
      cards[++startFrom]->timesWon += thisCardCount;
    }
  }

  return;
}
