#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "stack.h"
#include "player.h"
#include "list.h"

void addToStack(struct Stack *stack, void *data) {
    assert(stack != NULL);
    struct Node *newNode = calloc(1, sizeof(struct Node));
    assert(newNode != NULL);
    newNode->data = data;
    assert(newNode->data != NULL);

    stack->length++;
    if (stack->top == NULL) {
        stack->top = newNode;
        return;
    }

    newNode->next = stack->top;
    stack->top = newNode;

    return;
}

bool isStackEmpty(struct Stack *stack) {
    return stack->top == NULL;
}

struct Node *removeFromStack(struct Stack *stack) {
    assert(stack != NULL);

    if (stack->top == NULL) {
        return NULL;
    }

    stack->length--;
    struct Node *nodeToRemove = stack->top;
    stack->top = stack->top->next;

    return nodeToRemove;
}

void copyStackToQueue(struct Stack *stack, struct Queue *queue) {
    assert(stack != NULL && queue != NULL);
    struct Node *curent = stack->top;
    while (curent) {
        struct Team *teamToCopy = calloc(1, sizeof(struct Team));
        assert(teamToCopy != NULL);

        struct Team *teamCopied = curent->data;
        assert(curent->data != NULL);

        assert(teamCopied->name != NULL);
        teamToCopy->name = calloc(strlen(teamCopied->name), sizeof(char));
        assert(teamToCopy->name != NULL);
        strcpy(teamToCopy->name, teamCopied->name);

        teamToCopy->totalPoints = teamCopied->totalPoints;
        teamToCopy->teamSize = teamCopied->teamSize;
        enqueue(queue, teamToCopy);
        curent = curent->next;
    }
}

void deleteNodesFromStack(struct Node **node, void (*freeItem)(void *data)) {
    if (node == NULL || *node == NULL) {
        return;
    }

    freeItem((*node)->data);
    deleteNodesFromStack(&(*node)->next, freeItem);
    free(*node);
    *node = NULL;
}

void deleteStack(struct Stack *stack, void (*freeItem)(void *data)) {
    deleteNodesFromStack(&stack->top, freeItem);
}
