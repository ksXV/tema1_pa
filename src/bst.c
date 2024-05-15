#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "bst.h"

void addNodeToBST(struct BNode *node, struct BNode *nodeToAdd, uint points) {
    if (node == NULL) {
        return;
    }
    if (points == node->points) {
        struct Team *newTeam = nodeToAdd->data;
        struct Team *teamToCompare = node->data;
        if (strcmp(newTeam->name,teamToCompare->name) < 0) {
            if (node->left == NULL) {
                node->left = nodeToAdd;
                return;
            } else {
                nodeToAdd->left = node->left;
                node->left = nodeToAdd;
                return;
            }
        } else {
            if (node->right == NULL) {
                node->right = nodeToAdd;
                return;
            } else {
                nodeToAdd->right= node->right;
                node->right= nodeToAdd;
                return;
            }
        }
    } else if (points > node->points) {
        if (node->right == NULL) {
            node->right = nodeToAdd;
            return;
        }
        addNodeToBST(node->right, nodeToAdd, points);
    } else {
        if (node->left == NULL) {
            node->left = nodeToAdd;
            return;
        }
        addNodeToBST(node->left, nodeToAdd, points);
    }
}

void addToBST(struct BST *bst, void *data, uint points) {
    assert(bst != NULL);
    assert(data != NULL);

    struct BNode *newNode = calloc(1, sizeof(struct BNode));
    assert(newNode != NULL);

    newNode->data = data;
    newNode->points = points;

    if (bst->head == NULL) {
        bst->head = newNode;
        return;
    }
    addNodeToBST(bst->head, newNode, points);
}

