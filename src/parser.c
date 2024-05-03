#include "player.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define END_OF_LINE_WINDOWS "\r\n"

void parse_data(char *data, size_t dataLength, char *req, size_t reqLength) {
  char *p = strtok(data, END_OF_LINE_WINDOWS);
  int teamsLength = atoi(p);
  struct Team teams[teamsLength];
  memset(teams, 0, sizeof(teams));

  int teamIndex = -1;
  p = strtok(NULL, END_OF_LINE_WINDOWS);

  while (p) {
    if (*p >= '0' && *p <= '9') {
    teamIndex++;
      uint teamSize = atoi(p);
      int positionsToJump = 0;
      if (teamSize <= 9) {
        // include the whitespace
        positionsToJump = 2;
      } else {
        positionsToJump = 3;
      }
      teams[teamIndex].players =
          malloc(sizeof(struct Player) * teamSize);
      if (teams[teamIndex].players == NULL) {
        // handle err
        // for now just exit
        exit(1);
      }
      int titleLength = strlen(p + positionsToJump) + 1;
      teams[teamIndex].name = malloc(titleLength * sizeof(char));
      memmove(teams[teamIndex].name, p + positionsToJump, titleLength);

      goto getNextLine;
    }
    else {
        char *checkForNumber = p;
        while (*checkForNumber >= '9' || *checkForNumber <= '0') checkForNumber++;
        
        goto getNextLine;
    }

  getNextLine:
    p = strtok(NULL, END_OF_LINE_WINDOWS);
  }

}
