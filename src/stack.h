#include <stdio.h>
struct Stack {
    struct Node *top;
};

void addToStack(struct Stack *, void *data);
bool isStackEmpty(struct Stack *);
struct Node *removeFromStack(struct Stack *);
void deleteStack(struct Stack *, void(*freeData)(void *));
