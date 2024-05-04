#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

int addToList(struct Node **head, void *data, size_t dataSize) {
    struct Node *newNode = calloc(sizeof(struct Node), 1);
    if (newNode == NULL) {
        fprintf(stderr, "malloc failed!\n");
        return 1;
    }
    newNode->data = malloc(dataSize);
    if (newNode->data == NULL) {
        fprintf(stderr, "malloc failed for data!\n");
        return 1;
    }

    memmove(newNode->data, data, dataSize);
    if (*head == NULL) {
        *head = newNode;
        return 0;
    }

    newNode->next = *head;
    *head = newNode;

    return 0;
};

void printList(struct Node *head) {
    if (head == NULL) {
        putchar('\n');
        return;
    }
    printList(head->next);
    printf("%p\n", head);
}

void *popFromList(struct Node *head) {
    return NULL;
};
