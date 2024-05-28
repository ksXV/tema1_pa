#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "player.h"
#include "stack.h"
#include "bst.h"
#include "avl.h"

#define GET_TEAM(DATA) ((struct Team *) DATA)

struct TeamToBeElimated {
    struct Node* teamToBeElimated;
    int points;
};

void writeTeamsToOutput(struct Node *teams, FILE *output);
void eliminateTeamsFromList(struct Node **teams, uint totalTeams); 
void startMatches(struct Node *teams, FILE *output, struct Queue *topEightTeams);
void makeTopEightRanking(struct Queue *topEightTeams, FILE *output);
void getSecondLevelRanking(struct Queue *topEightTeams, FILE *output);

void freeTeams(struct Node *teams) {
    assert(teams != NULL);
    struct Node *curent = teams;
    while (curent) {
        struct Node *aux = curent;
        curent = curent->next;
        freeTeam(aux->data);
        free(aux);
    }
}

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
        freeTeams(teams);
        return;
    }

    if (req[0]) {
        writeTeamsToOutput(teams, output);
        freeTeams(teams);
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
    free(node);
}

void BFS(struct AVLNode *node, FILE *output, struct Team* teamsToOutput[4]) {
    struct Queue q = {.head = NULL, .tail = NULL};
    enqueue(&q, node);
    int idx = 0;
    while (!isQueueEmpty(&q)) {
        struct Node *foo = dequeue(&q);
        struct AVLNode *t = foo->data;
        if (idx < 3) {
            teamsToOutput[idx++] = GET_TEAM(t->data);
        } else if (isQueueEmpty(&q)) {
            teamsToOutput[idx] = GET_TEAM(t->data);
            return;
        }
        if (t->left) {
            enqueue(&q, t->left);
        }
        if (t->right) {
            enqueue(&q, t->right);
        }
        free(foo);
        free(t);
    }
}

bool howToInsert(void *nodeOne, void *nodeTwo) {
    struct Team *teamOne = nodeOne;
    struct Team *teamTwo = nodeTwo;
    if (teamOne->totalPoints != teamTwo->totalPoints) {
        return teamOne->totalPoints > teamTwo->totalPoints;
    } else {
        return strcmp(teamOne->name, teamTwo->name) > 0;
    }
}

int howToSort(const void *nodeOne, const void *nodeTwo) {
    const struct Team *teamOne = *(const void **)nodeOne;
    const struct Team *teamTwo = *(const void **)nodeTwo;
    if (teamOne->totalPoints != teamTwo->totalPoints) {
        return teamOne->totalPoints - teamTwo->totalPoints;
    } else {
        return strcmp(teamOne->name, teamTwo->name);
    }
}

void getSecondLevelRanking(struct Queue *topEightTeams, FILE *output) {
    assert(topEightTeams != NULL);
    assert(output != NULL);
    struct Node *curent = topEightTeams->head;
    while (curent) {
        curent = curent->next;
    }
    struct AVL tree = {.head = NULL, .fn = howToInsert};
    int idx = 0;
    struct Team *foo[8];
    while (!isQueueEmpty(topEightTeams)) { 
        struct Node *teamNode = dequeue(topEightTeams);
        assert(teamNode != NULL);
        struct Team *team = teamNode->data;
        assert(team != NULL);
        foo[idx++] = team;
    }
    qsort(foo, 8, sizeof(struct Team *), howToSort);

    for (int i = 0; i < 8; i++) {
        tree.head = addToAVL(&tree, foo[i]);
    }

    fprintf(output, "\nTHE LEVEL 2 TEAMS ARE:\n");
    struct Team* teamsToOutput[4];
    BFS(tree.head, output, teamsToOutput);

    fprintf(output, "%s\n", teamsToOutput[3]->name);
    fprintf(output, "%s\n", teamsToOutput[1]->name);
    fprintf(output, "%s\n", teamsToOutput[0]->name);
    fprintf(output, "%s\n", teamsToOutput[2]->name);
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
        free(teamNode);
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

    while (curent && curent->next) {
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
            free(possibleTeamOne);
            struct Node *possibleTeamTwo = dequeue(&queue);
            if (possibleTeamTwo == NULL) {
                freeTeam(firstTeam);
                return;
            } else if (shouldPrintRoundNumber) {
                fprintf(output, "\n--- ROUND NO:%d\n", roundNumber);
                shouldPrintRoundNumber = false;
            }
            struct Team *secondTeam = possibleTeamTwo->data;
            free(possibleTeamTwo);
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
            free(winner);
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
