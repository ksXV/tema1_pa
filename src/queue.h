#include <stdio.h>

struct Queue {
    struct Node *head;
    struct Node *tail;
};

void enqueue(struct Queue *, void *);
struct Node *dequeue(struct Queue *);
bool isQueueEmpty(struct Queue *);
