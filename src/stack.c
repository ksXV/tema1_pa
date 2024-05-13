#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "stack.h"
#include "list.h"

void addToStack(struct Stack *stack, void *data) {
    assert(stack != NULL);
    struct Node *newNode = calloc(1, sizeof(struct Node));
    assert(newNode != NULL);
    newNode->data = data;
    assert(newNode->data != NULL);

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

    struct Node *nodeToRemove = stack->top;
    stack->top = stack->top->next;

    return nodeToRemove;
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
