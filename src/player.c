#include <stdlib.h>
#include <stdio.h>
#include "player.h"

void freePlayer(void *player) {
    if (player) {
        struct Player *playerToFree = player;
        if (playerToFree->firstName)
            free(playerToFree->firstName);

        if (playerToFree->firstName)
            free(playerToFree->secondName);
    }
};

void freeTeam(void *team) {
    if (team) {
        struct Team *teamToFree = team;
        if (teamToFree->name) {
            free(teamToFree->name);
        }
        if (teamToFree->players) {
            for (int i = 0; i < teamToFree->teamSize; i++) {
                freePlayer(&teamToFree->players[i]);
            }
            free(teamToFree->players);
        }
        free(team);
        team = NULL;
    }
};
