#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"
#include "player.h"

void freeNode(struct Node *item) {
    if (item->data) {
        free(item->data);
        item->data = 0;
    }
    free(item);
    item = 0;
}

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

void removeFromList(struct Node **head, struct Node *item) {
    assert(head != NULL);
    assert(item != NULL);
    if (*head == NULL) return;
    if (*head == item) {
        struct Node *next = (*head)->next;
        freeNode(*head);
        *head = next;
        return;
    }
    struct Node *curent = *head;
    while (curent && curent->next != item) {
        /* printf("%p %p\n", curent, item); */
        curent = curent->next;
    }

    struct Node *nodeToDelete = curent->next;

    curent->next = nodeToDelete->next;
    freeNode(nodeToDelete);
    nodeToDelete=0;
    return;
};
