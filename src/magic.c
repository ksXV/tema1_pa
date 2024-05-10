#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "list.h"
#include "player.h"
#include "queue.h"
#include "stack.h"

struct TeamToBeElimated {
    struct Node* teamToBeElimated;
    int points;
};

void writeTeamsToOutput(struct Node *teams, FILE *output);
void eliminateTeamsFromList(struct Node **teams, uint totalTeams); 
void startMatches(struct Node *teams, FILE *output);

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
        eliminateTeamsFromList(&teams, totalTeams);
        writeTeamsToOutput(teams, output);
        startMatches(teams, output);

        return;
    }

    if (req[1]) {
        eliminateTeamsFromList(&teams, totalTeams);
        writeTeamsToOutput(teams, output);
        return;
    }

    if (req[0]) {
        writeTeamsToOutput(teams, output);
        return;
    }
}

void startMatches(struct Node *teams, FILE *output) {
    struct Node *curent = teams;
    struct Queue queue = {.head = NULL, .tail = NULL};

    while(curent && curent->next) {
        struct Match *match = calloc(1, sizeof(struct Match));
        assert(match != NULL);
        match->teamOne = curent->data;
        match->teamTwo = curent->next->data;
        enqueue(&queue, match, 0);
        curent = curent->next->next;
    }
    
    struct Stack winners = {.top = NULL};
    struct Stack losers = {.top = NULL};
    
    fprintf(output, "\n--- ROUND NO:1\n");
    putchar('\n');
    while (!isQueueEmpty(&queue)) { 
        struct Node *possibleMatch = dequeue(&queue);
        assert(possibleMatch != NULL);
        struct Match *match = possibleMatch->data;
        assert(match != NULL); 
        struct Team *firstTeam = match->teamOne;
        struct Team *secondTeam = match->teamTwo;
        assert(firstTeam != NULL && secondTeam != NULL);
        if ((firstTeam->totalPoints / firstTeam->teamSize) >= (secondTeam->totalPoints / secondTeam->teamSize)) {
            addToStack(&winners, firstTeam);
            addToStack(&losers, secondTeam);
        } else {
            addToStack(&winners, secondTeam);
            addToStack(&losers, firstTeam);
        }
        fprintf(output, "%s - %s\n", firstTeam->name, secondTeam->name);
    }
    fprintf(output, "\nWINNERS OF ROUND NO:1\n");
    while (!isStackEmpty(&winners)) {
        struct Node *possibleMatch = removeFromStack(&winners);
        assert(possibleMatch != NULL);
        struct Team *winnerTeam = possibleMatch->data;
        if (winnerTeam == NULL) {
            printf("seg faulthere\n");
            break;
        };
        float points = winnerTeam->totalPoints / (float) winnerTeam->teamSize;
        fprintf(output, "%s - %.2f\n", winnerTeam->name, points + 1.00);
        winnerTeam->totalPoints += winnerTeam->teamSize;
    }
    deleteStack(&losers, freeTeam);
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

void eliminateTeamsFromList(struct Node **teams, uint totalTeams) {
    assert(teams != NULL);
    assert(*teams != NULL);

    int teamsToEliminate = 1;
    while (teamsToEliminate < totalTeams) teamsToEliminate *= 2;
    teamsToEliminate = teamsToEliminate / 2;

    while (totalTeams != teamsToEliminate) {
        assert((*teams)->next != NULL);
        assert((*teams)->data != NULL);

        //skip the first team 
        struct Node *current = (*teams)->next;
        struct Team *currentTeam = current->data;

        struct TeamToBeElimated team = {.teamToBeElimated = *teams, .points = ((struct Team *)(*teams)->data)->totalPoints};

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

        removeFromList(teams, team.teamToBeElimated);
        totalTeams--;
    }
}
