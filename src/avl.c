#include "avl.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "player.h"

int height(struct AVLNode *N) { 
    if (N == NULL) 
        return 0; 
    return N->height; 
} 

int max(uint a, uint b) { 
    return a > b ? a : b; 
} 

struct AVLNode *rightRotate(struct AVLNode *y) { 
    struct AVLNode *x = y->left; 
    struct AVLNode *T2 = x->right; 
  
    x->right = y; 
    y->left = T2; 
  
    y->height = max(height(y->left), 
                    height(y->right)) + 1; 
    x->height = max(height(x->left), 
                    height(x->right)) + 1; 
  
    return x; 
} 

struct AVLNode *leftRotate(struct AVLNode *x) { 
    struct AVLNode *y = x->right; 
    struct AVLNode *T2 = y->left; 
  
    y->left = x; 
    x->right = T2; 
  
    x->height = max(height(x->left),    
                    height(x->right)) + 1; 
    y->height = max(height(y->left), 
                    height(y->right)) + 1; 
  
    return y; 
} 

 int getBalance(struct AVLNode *N) { 
    if (N == NULL) 
        return 0; 
    return height(N->left) - height(N->right); 
} 

struct AVLNode *insert(struct AVLNode *node, struct AVLNode *nodeToAdd, itemCompareFunction cmp) { 
    if (node == NULL) {
        return nodeToAdd;
    }
    if (cmp(node, nodeToAdd)) {
        node->right = insert(node->right, nodeToAdd, cmp);
    } else {
        node->left = insert(node->left, nodeToAdd, cmp);
    }
  
    node->height = 1 + max(height(node->left), 
                        height(node->right)); 
  
    int balance = getBalance(node); 
  
    if (balance > 1 && cmp(nodeToAdd, node->left))
        return rightRotate(node); 
  
    if (balance < -1 && !cmp(nodeToAdd, node->right))
        return leftRotate(node); 
  
    if (balance > 1 && !cmp(nodeToAdd, node->left)) { 
        node->left =  leftRotate(node->left); 
        return rightRotate(node); 
    } 
  
    if (balance < -1 && cmp(nodeToAdd, node->right)) { 
        node->right = rightRotate(node->right); 
        return leftRotate(node); 
    } 
  
    return node; 
} 

struct AVLNode *addToAVL(struct AVL *avl, void *data) {
    assert(avl != NULL);
    assert(data != NULL);

    struct AVLNode *newNode = calloc(1, sizeof(struct AVLNode));
    assert(newNode != NULL);

    newNode->data = data;
    newNode->height = 1;

    if (avl->head == NULL) {
        return newNode;
    }
    return insert(avl->head, newNode, avl->fn);
}
