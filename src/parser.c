#include "list.h"
#include "player.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define END_OF_LINE_WINDOWS "\r\n"

void debug_printTeams(struct Node *teams);

struct Node *parseData(char *data) {
  assert(data != NULL);
  char *p = strtok(data, END_OF_LINE_WINDOWS);

  int playerIndex = 0;

  struct Node *teams = NULL;

  p = strtok(NULL, END_OF_LINE_WINDOWS);

  while (p) {
    if (*p >= '0' && *p <= '9') {
      uint teamSize = atoi(p);
      int positionsToJump = 0;
      playerIndex = 0;

      if (teamSize <= 9) {
        // include the whitespace
        positionsToJump = 2;
      } else {
        positionsToJump = 3;
      }
      struct Team *newTeam = calloc(sizeof(struct Team), 1);
      assert(newTeam != NULL);
      newTeam->players = calloc(sizeof(struct Player), teamSize);
      assert(newTeam->players != NULL);
      newTeam->teamSize = teamSize;
      int titleLength = strlen(p + positionsToJump) + 1;
      newTeam->name = calloc(sizeof(char), titleLength);
      memmove(newTeam->name, p + positionsToJump, titleLength);

      int status = addToList(&teams, newTeam, sizeof(struct Team));
      assert(status == 0);

      goto getNextLine;
    } else {
      assert(teams->data != NULL);
      struct Team *currentTeam = (struct Team *)teams->data;
      struct Player *currentPlayer = &currentTeam->players[playerIndex];
      char *checkForWhiteSpace = p;
      while (*checkForWhiteSpace != ' ')
        checkForWhiteSpace++;
      currentPlayer->firstName =
          calloc(sizeof(char), (checkForWhiteSpace - p + 1));
      assert(currentPlayer->firstName != NULL);
      memmove(currentPlayer->firstName, p, checkForWhiteSpace - p);

      checkForWhiteSpace++;

      char *checkForDigit = checkForWhiteSpace;
      while (!isdigit(*checkForDigit))
        checkForDigit++;
      currentPlayer->secondName =
          calloc(sizeof(char), (checkForWhiteSpace - p + 1));
      assert(currentPlayer->secondName != NULL);
      memmove(currentPlayer->secondName, checkForWhiteSpace,
              checkForDigit - checkForWhiteSpace - 1);
      uint points = atoi(checkForDigit);
      currentPlayer->points = points;

      playerIndex++;
      goto getNextLine;
    }

  getNextLine:
    p = strtok(NULL, END_OF_LINE_WINDOWS);
  }
  debug_printTeams(teams);

  return teams;
}

int* parseReq(char *req) {
    assert(req != NULL);
    static int requirements[5] = {};
    char *p = strtok(req, " ");
    int i = 0;
    while (p && i < 5) {
        requirements[i] = atoi(p);
        i++;
        p = strtok(NULL, " ");
    }

    for (int i = 0; i < 5; i++) {
        printf("%d ", requirements[i]);
    }
    putchar('\n');

    return requirements;
}

void debug_printTeams(struct Node *teams) {
  if (!teams)
    return;
  debug_printTeams(teams->next);
  struct Team *currentTeam = (struct Team *)teams->data;
  assert(currentTeam != NULL);
  assert(currentTeam->name != NULL);
  puts(currentTeam->name);
  assert(currentTeam->players != NULL);
  for (int i = 0; i < currentTeam->teamSize; i++) {
    struct Player currentPlayer = currentTeam->players[i];
    assert(currentPlayer.firstName != NULL);
    printf("\t%s ", currentPlayer.firstName);
    assert(currentPlayer.secondName != NULL);
    printf("%s ", currentPlayer.secondName);
    printf("%d\n", currentPlayer.points);
  }
}
