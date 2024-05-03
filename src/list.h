#include <stdlib.h>
struct Node {
    struct Node *next;
    void *data;
};

int addToList(struct Node **head, void *data, size_t);
void *popFromList(struct Node *head);
void printList(struct Node *head);