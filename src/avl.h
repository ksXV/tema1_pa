#include <sys/types.h>
#include <stdbool.h>

struct AVLNode {
    struct AVLNode *left;
    struct AVLNode *right;
    void *data;
    int height;
};


typedef bool (*itemCompareFunction)(void*, void*);

struct AVL {
    struct AVLNode *head;
    itemCompareFunction fn;   
};

struct AVLNode *addToAVL(struct AVL *, void *data);
