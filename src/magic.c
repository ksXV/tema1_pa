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
        enqueue(&queue,curent->data, 0);
        enqueue(&queue, curent->next->data, 0);
        curent = curent->next->next;
    }
    
    struct Stack *winners = calloc(1, sizeof(struct Stack));
    struct Stack *losers = calloc(1, sizeof(struct Stack));
    assert(winners != NULL && losers != NULL);
    int roundNumber = 1;
    while (1) {
        fprintf(output, "\n--- ROUND NO:%d\n", roundNumber);
        while (!isQueueEmpty(&queue)) { 
            struct Node *possibleTeamOne = dequeue(&queue);
            assert(possibleTeamOne != NULL);
            struct Team *firstTeam = possibleTeamOne->data;

            struct Node *possibleTeamTwo = dequeue(&queue);
            if (possibleTeamTwo == NULL) {
                /* printf("%s\n", firstTeam->name); */
                return;
            }
            struct Team *secondTeam = possibleTeamTwo->data;
            assert(firstTeam != NULL && secondTeam != NULL);
            if ((firstTeam->totalPoints / firstTeam->teamSize) >= (secondTeam->totalPoints / secondTeam->teamSize)) {
                addToStack(winners, firstTeam);
                addToStack(losers, secondTeam);
            } else {
                addToStack(winners, secondTeam);
                addToStack(losers, firstTeam);
            }
            fprintf(output, "%s - %s\n", firstTeam->name, secondTeam->name);
        }
        fprintf(output, "\nWINNERS OF ROUND NO:%d\n", roundNumber);
        while (!isStackEmpty(winners)) {
            struct Node *winner = removeFromStack(winners);
            assert(winner != NULL);
            struct Team *winnerTeam = winner->data;
            assert (winnerTeam != NULL);
            float points = winnerTeam->totalPoints / (float) winnerTeam->teamSize;
            fprintf(output, "%s - %.2f\n", winnerTeam->name, points + 1.00);
            winnerTeam->totalPoints += winnerTeam->teamSize;
            enqueue(&queue, winnerTeam, 0);
        }
        deleteStack(losers, freeTeam);
        putchar('\n');
        printf("%p\n", losers->top);
        putchar('\n');
        roundNumber++;
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
