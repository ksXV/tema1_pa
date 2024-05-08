#include "list.h"
#include "player.h"
#include <assert.h>
#include <stdio.h>

struct TeamToBeElimated {
    struct Node* teamToBeElimated;
    int points;
};

void writeTeamsToOutput(struct Node *teams, FILE *output);
void eliminateTeamsFromList(struct Node *teams, FILE *output, uint totalTeams); 

void handleRequirements(struct Node* teams, int *req, FILE *output, uint totalTeams) {
    assert(teams != NULL);
    assert(req != NULL);
    assert(output != NULL);
    assert(totalTeams != 0);

    if (req[4]) {

        return;
    }

    if (req[3]) {

        return;
    }

    if (req[2]) {

        return;
    }

    if (req[1]) {
        eliminateTeamsFromList(teams, output, totalTeams);
        return;
    }

    if (req[0]) {
        writeTeamsToOutput(teams, output);
        return;
    }
}

void writeTeamsToOutput(struct Node *teams, FILE *output) {
    assert(output != NULL);
    if (teams == NULL) {
        return;
    } 
    struct Team *currentTeam = (struct Team *) teams->data;
    fprintf(output, "%s\n", currentTeam->name);
    writeTeamsToOutput(teams->next, output);
}

void eliminateTeamsFromList(struct Node *teams, FILE *output, uint totalTeams) {
    assert(output != NULL);
    assert(teams != NULL);

    int teamsToEliminate = 1;
    while (teamsToEliminate < totalTeams) teamsToEliminate *= 2;
    teamsToEliminate = teamsToEliminate / 2;

    while (totalTeams != teamsToEliminate) {
        assert(teams->next != NULL);
        assert(teams->data != NULL);

        //skip the first team 
        struct Node *current = teams->next;
        struct Team *currentTeam = current->data;

        struct TeamToBeElimated team = {.teamToBeElimated = teams, .points = ((struct Team *)teams->data)->totalPoints};

        while (current) {
            if (team.points > currentTeam->totalPoints) {
                team.points = ((struct Team *) current->data)->totalPoints;
                team.teamToBeElimated = current;
            }
            current = current->next;
            if (current) {
                currentTeam = current->data;
            }
        }

        removeFromList(&teams, team.teamToBeElimated);
        totalTeams--;
    }
    writeTeamsToOutput(teams, output);
}
