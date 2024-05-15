#include <stdio.h>
#include "queue.h"

struct Stack {
    struct Node *top;
    size_t length;
};

void addToStack(struct Stack *, void *data);
bool isStackEmpty(struct Stack *);
struct Node *removeFromStack(struct Stack *);
void deleteStack(struct Stack *, void(*)(void *));
void copyStackToQueue(struct Stack *stack, struct Queue *queue); 
