#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "player.h"
#include "stack.h"
#include "bst.h"
#include "avl.h"

struct TeamToBeElimated {
    struct Node* teamToBeElimated;
    int points;
};

void writeTeamsToOutput(struct Node *teams, FILE *output);
void eliminateTeamsFromList(struct Node **teams, uint totalTeams); 
void startMatches(struct Node *teams, FILE *output, struct Queue *topEightTeams);
void makeTopEightRanking(struct Queue *topEightTeams, FILE *output);
void getSecondLevelRanking(struct Queue *topEightTeams, FILE *output);

void handleRequirements(struct Node* teams, int *req, FILE *output, uint totalTeams) {
    assert(teams != NULL);
    assert(req != NULL);
    assert(output != NULL);
    assert(totalTeams != 0);

    if (req[4]) {
        struct Queue topEightTeams = {.head = NULL, .tail = NULL};
        eliminateTeamsFromList(&teams, totalTeams);
        writeTeamsToOutput(teams, output);
        startMatches(teams, output, &topEightTeams);
        makeTopEightRanking(&topEightTeams, output);
        getSecondLevelRanking(&topEightTeams, output);
        return;
    }

    if (req[3]) {
        struct Queue topEightTeams = {.head = NULL, .tail = NULL};
        eliminateTeamsFromList(&teams, totalTeams);
        writeTeamsToOutput(teams, output);
        startMatches(teams, output, &topEightTeams);
        makeTopEightRanking(&topEightTeams, output);
        return;
    }

    if (req[2]) {
        eliminateTeamsFromList(&teams, totalTeams);
        writeTeamsToOutput(teams, output);
        startMatches(teams, output, NULL);
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

void DFSPostOrder(struct BNode *node, FILE *output) {
    assert(output != NULL);
    if (node == NULL) return;

    DFSPostOrder(node->right, output);
    struct Team *team = node->data;
    assert(team != NULL);
    float points = team->totalPoints / (float) team->teamSize;
    fprintf(output, "%-33s -  %.2f\n", team->name, points + 1);
    DFSPostOrder(node->left, output);
}


bool howToInsert(void *node, void *nodeToAdd) {
    struct Team *curentNode = node;
    struct Team *nodeToAddAvl = nodeToAdd;
    return nodeToAddAvl->totalPoints < curentNode->totalPoints;
}

void getLastTeam(struct AVLNode *node, int level, FILE *output) {
    level++;
    if (node->right!= NULL) {
        getLastTeam(node->right, level, output);
        if (level == 3) {
            struct Team *team = node->data;
            fprintf(output, "%s\n", team->name);
            return;
        }
    }
}

void howToPrintTeams(FILE *output, void *data) {
    assert(data != NULL);
    struct Team *team = data;
    fprintf(output, "%s\n", team->name); 

}

void printTeams(struct AVL *avl, FILE *output) {
    getFirstThreeTeams(avl->head, output, howToPrintTeams);
    getLastTeam(avl->head, 0, output);
}

void getSecondLevelRanking(struct Queue *topEightTeams, FILE *output) {
    assert(topEightTeams != NULL);
    assert(output != NULL);
    struct Node *curent = topEightTeams->head;
    while (curent) {
        curent = curent->next;
    }
    struct AVL tree = {.head = NULL, .fn = howToInsert};
    struct Team *teams[8] = {};
    int idxTeams = 0;
    while (!isQueueEmpty(topEightTeams)) { 
        struct Node *teamNode = dequeue(topEightTeams);
        assert(teamNode != NULL);
        struct Team *team = teamNode->data;
        assert(team != NULL);
        team->totalPoints *= 10;
        teams[idxTeams++] = team;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (teams[i]->totalPoints > teams[j]->totalPoints) {
                struct Team *aux = teams[i];
                teams[i] = teams[j];
                teams[j] = aux;
            } else if (teams[i]->totalPoints == teams[j]->totalPoints && strcmp(teams[i]->name, teams[j]->name) > 0) {
                struct Team *aux = teams[i];
                teams[i] = teams[j];
                teams[j] = aux;
            }
        }
    }

    int pointsToSubtract = 8;
    for (idxTeams = 0; idxTeams < 8; idxTeams++) {
        if (idxTeams + 1 < 8 && teams[idxTeams]->totalPoints == teams[idxTeams + 1]->totalPoints) {
            teams[idxTeams]->totalPoints += pointsToSubtract;
            pointsToSubtract--;
        }
        tree.head = addToAVL(&tree, teams[idxTeams]);
    }

    fprintf(output, "\nTHE LEVEL 2 TEAMS ARE:\n");
    printTeams(&tree, output);
}
void makeTopEightRanking(struct Queue *topEightTeams, FILE *output) {
    assert(topEightTeams != NULL);
    assert(output != NULL);
    struct BST tree = {.head = NULL};
    int idx = 0;
    while (8 > idx) { 
        struct Node *teamNode = dequeue(topEightTeams);
        assert(teamNode != NULL);
        struct Team *team = teamNode->data;
        assert(team != NULL);
        addToBST(&tree, team, team->totalPoints);
        enqueue(topEightTeams, team);
        idx++;
    }

    fprintf(output, "\nTOP 8 TEAMS:\n");
    DFSPostOrder(tree.head, output);
}

void startMatches(struct Node *teams, FILE *output, struct Queue *topEightTeams) {
    assert(output != NULL);
    assert(teams != NULL);

    struct Node *curent = teams;
    struct Queue queue = {.head = NULL, .tail = NULL};

    while(curent && curent->next) {
        enqueue(&queue,curent->data);
        enqueue(&queue, curent->next->data);
        curent = curent->next->next;
    }
    
    struct Stack winners = {.top = NULL, .length = 0};
    struct Stack losers = {.top = NULL, .length = 0};
    int roundNumber = 1;
    bool shouldPrintRoundNumber = true;
    while (1) {
        while (!isQueueEmpty(&queue)) { 
            struct Node *possibleTeamOne = dequeue(&queue);
            assert(possibleTeamOne != NULL);
            struct Team *firstTeam = possibleTeamOne->data;
            struct Node *possibleTeamTwo = dequeue(&queue);
            if (possibleTeamTwo == NULL) {
                return;
            } else if (shouldPrintRoundNumber) {
                fprintf(output, "\n--- ROUND NO:%d\n", roundNumber);
                shouldPrintRoundNumber = false;
            }
            struct Team *secondTeam = possibleTeamTwo->data;
            assert(firstTeam != NULL && secondTeam != NULL);
            if ((firstTeam->totalPoints / (float) firstTeam->teamSize) > (secondTeam->totalPoints / (float) secondTeam->teamSize)) {
                addToStack(&winners, firstTeam);
                addToStack(&losers, secondTeam);
            } else if ((firstTeam->totalPoints / (float) firstTeam->teamSize) < (secondTeam->totalPoints / (float) secondTeam->teamSize)) {
                addToStack(&winners, secondTeam);
                addToStack(&losers, firstTeam);
            } else {
                addToStack(&losers, firstTeam);
                addToStack(&winners, secondTeam);
            }
            fprintf(output, "%-32s -%33s\n", firstTeam->name, secondTeam->name);
        }

        if (winners.length == 8 && topEightTeams != NULL) {
            copyStackToQueue(&winners, topEightTeams);
        }


        fprintf(output, "\nWINNERS OF ROUND NO:%d\n", roundNumber);
        while (!isStackEmpty(&winners)) {
            struct Node *winner = removeFromStack(&winners);
            assert(winner != NULL);
            struct Team *winnerTeam = winner->data;
            assert (winnerTeam != NULL);
            float points = winnerTeam->totalPoints / (float) winnerTeam->teamSize;
            fprintf(output, "%-33s -  %.2f\n", winnerTeam->name, points + 1.00);
            winnerTeam->totalPoints += winnerTeam->teamSize;
            enqueue(&queue, winnerTeam);
        }
        deleteStack(&losers, freeTeam);
        roundNumber++;
        shouldPrintRoundNumber = true;
    }
}

void writeTeamsToOutput(struct Node *teams, FILE *output) {
    assert(output != NULL);
    if (teams == NULL) {
        return;
    } 
    struct Team *currentTeam = teams->data;
    fprintf(output, "%s\n", currentTeam->name);
    writeTeamsToOutput(teams->next, output);
}

void eliminateTeamsFromList(struct Node **teams, uint totalTeams) {
    assert(teams != NULL);
    assert(*teams != NULL);

    int teamsToEliminate = 1;
    while (teamsToEliminate <= totalTeams) teamsToEliminate *= 2;
    teamsToEliminate = teamsToEliminate / 2;

    while (totalTeams > teamsToEliminate) {
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
