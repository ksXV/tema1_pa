#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"
#include "list.h"
#include "player.h"

void enqueue(struct Queue *q, void *data, size_t dataLength) {
    assert(q != NULL); 
    struct Node *newNode = calloc(1, sizeof(struct Node));
    assert(newNode != NULL);
    newNode->data = data;
    if (q->tail == NULL) {
        q->tail = newNode;
    } else {
        q->tail->next = newNode;
        q->tail = newNode;
    }

    if (q->head == NULL) q->head = q->tail;
}; 

bool isQueueEmpty(struct Queue *queue) {
    return queue->head == NULL || queue->tail == NULL;
}

struct Node *dequeue(struct Queue *q) {
    assert(q != NULL); 
    struct Node* nodeToDequeue = q->head;

    if (q->head == NULL) {
        q->tail = NULL;
        return NULL;
    }

    q->head = q->head->next;

    //it may slip that the tail might not become null;
    if (q->head == NULL) {
        q->tail = NULL;
    }

    return nodeToDequeue;
};
