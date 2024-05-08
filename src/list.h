#include <stdlib.h>
struct Node {
    struct Node *next;
    void *data;
};

int addToList(struct Node **head, void *data, size_t);
void popFromList(struct Node **head, struct Node *item);
void removeFromList(struct Node **head, struct Node *item);
void printList(struct Node *head);
