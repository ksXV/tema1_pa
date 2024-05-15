#include <sys/types.h>

struct BNode {
    struct BNode *left;
    struct BNode *right;
    uint points;
    void *data;
};

struct BST {
    struct BNode *head;
};
void addToBST(struct BST *bst, void *data, uint points);
