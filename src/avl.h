#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>

struct AVLNode {
    struct AVLNode *left;
    struct AVLNode *right;
    uint points;
    void *data;
    int height;
};


typedef bool (*itemCompareFunction)(void*, void*);
typedef void (*printTheTeams)(FILE *, void*);

struct AVL {
    struct AVLNode *head;
    itemCompareFunction fn;   
};

struct AVLNode *addToAVL(struct AVL *, void *data);
void getFirstThreeTeams(struct AVLNode *node, FILE *output, printTheTeams fn);
