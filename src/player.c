#include <stdlib.h>
#include "player.h"

void freePlayer(void *player) {
    if (player) {
        struct Player *player = player;
        free(player->firstName);
        free(player->secondName);
        free(player);
        player = NULL;
    }
};

void freeTeam(void *team) {
    if (team) {
        struct Team *team = team;
        free(team->name);
        if (team->players) {
            for (int i = 0; i < team->teamSize; i++) {
                freePlayer(&team->players[i]);
            }
        }
        free(team);
        team = NULL;
    }
};
