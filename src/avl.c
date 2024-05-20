#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "avl.h"

int height(struct AVLNode *N) { 
    if (N == NULL) 
        return 0; 
    return N->height; 
} 

int max(int a, int b) { 
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
    if (cmp(node->data, nodeToAdd->data)) {
        node->right = insert(node->right, nodeToAdd, cmp);
    } else {
        node->left = insert(node->left, nodeToAdd, cmp);
    }
  
    node->height = 1 + max(height(node->left), 
                        height(node->right)); 
  
    int balance = getBalance(node); 
  
    if (balance > 1 && cmp(nodeToAdd->data, node->data))
        return rightRotate(node); 
  
    if (balance < -1 && !cmp(nodeToAdd->data, node->right->data))
        return leftRotate(node); 
  
    if (balance > 1 && !cmp(nodeToAdd->data, node->left->data)) { 
        node->left =  leftRotate(node->left); 
        return rightRotate(node); 
    } 
  
    if (balance < -1 && cmp(nodeToAdd->data, node->right->data)) { 
        node->right = rightRotate(node->right); 
        return leftRotate(node); 
    } 
  
    return node; 
} 


int getTheAVLHeight(struct AVLNode *node) {
    int hl,hr;
    if (node == NULL) return 0;
    hl = getTheAVLHeight(node->left);
    hr = getTheAVLHeight(node->right);

    return 1+((hl>hr)? hl:hr);
}

int idx = 0;
void getFirstThreeTeams(struct AVLNode *node, FILE *output, printTheTeams fn) {
    if (node->left != NULL) {
        if (getTheAVLHeight(node->left) == 1) idx++;
        getFirstThreeTeams(node->left, output, fn);
    }
    if (getTheAVLHeight(node) == 1 && idx < 4){
        fn(output, node->data);
    }
    if (node->right != NULL){
        if (getTheAVLHeight(node->right) == 1) idx++;   
        getFirstThreeTeams(node->right, output, fn);
    }
}

struct AVLNode *addToAVL(struct AVL *avl, void *data) {
    assert(avl != NULL);
    assert(data != NULL);

    struct AVLNode *newNode = calloc(1, sizeof(struct AVLNode));
    assert(newNode != NULL);

    newNode->data = data;
    newNode->height = 1;

    return insert(avl->head, newNode, avl->fn);
}
