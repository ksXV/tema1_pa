#include "list.h"
#include "player.h"
#include <assert.h>
#include <stdio.h>

void writeTeamsToOutput(struct Node *teams, FILE *output);

void handleRequirements(struct Node* teams, int *req, FILE *output) {
    assert(teams != NULL);
    assert(req != NULL);
    assert(output != NULL);

    if (req[0]) {
        writeTeamsToOutput(teams, output);
    }

    if (req[0]) {

    }

    if (req[0]) {

    }

    if (req[0]) {

    }

    if (req[0]) {

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
