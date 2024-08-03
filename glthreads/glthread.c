#include "glthread.h"
#include <stdlib.h>
#include <stdio.h>

void glthread_add_next(glthread *base, glthread *node) {
    if (!base->right) {
        base->right = node;
        node->left = base;
        return;
    }

    node->right = base->right;
    base->right->left = node;
    base->right = node;
    node->left = base;
}

void glthread_add_before(glthread *base, glthread *node) {
    if (!base->left) {
        node->left = NULL;
        node->right = base;
        base->left = node;
        return;
    }

    node->right = base;
    node->left = base->left;
    base->left->right = node;
    base->left = node;
}

void glthread_add_last(glthread *base, glthread *node) {
    glthread *ptr = NULL, *prev = NULL;

    ITERATE_GLTHREAD_BEGIN(base, ptr) {
        prev = ptr;
    } ITERATE_GLTHREAD_END(base, ptr);
    
    if (prev) glthread_add_next(prev, node);
    else glthread_add_next(base, node);
}

void remove_glthread(glthread *node) {
    if (!node->left) {
        if (node->right) {
            node->right->left = NULL;
            node->right = 0;
            return;
        }
        return;
    }

    if (!node->right) {
        node->left->right = NULL;
        node->left = 0;
        return;
    }

    node->left->right = node->right;
    node->right->left = node->left;
    node->right = 0;
    node->left = 0;
}

void init_glthread(glthread *node) {
    node->left = NULL;
    node->right = NULL;
}

void delete_glthread_lst(glthread *start_ptr) {
    glthread *ptr = NULL;
    ITERATE_GLTHREAD_BEGIN(start_ptr, ptr) {
        remove_glthread(ptr);
    } ITERATE_GLTHREAD_END(start_ptr, ptr);
}

unsigned int get_glthread_lst_count(glthread *base) {
    int count = 0;
    glthread *ptr = NULL;
    ITERATE_GLTHREAD_BEGIN(base, ptr) {
        count++;
    } ITERATE_GLTHREAD_END(base, ptr);
    return count;
}

void glthread_priority_insert(glthread *base, glthread *node, int (*comp_fn)(void *, void *), int offset) {
    glthread *prev = NULL, *curr = NULL;

    init_glthread(node);

    if(IS_GLTHREAD_LIST_EMPTY(base)) {
        glthread_add_next(base, node);
        return;
    }
    /* Only one node*/
    if(base->right && !base->right->right){
        if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(base->right, offset), 
                GLTHREAD_GET_USER_DATA_FROM_OFFSET(node, offset)) == -1){
            glthread_add_next(base->right, node);
        }
        else{
            glthread_add_next(base, node);
        }
        return;
    }

    if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(node, offset), 
            GLTHREAD_GET_USER_DATA_FROM_OFFSET(base->right, offset)) == -1){
        glthread_add_next(base, node);
        return;
    }

    ITERATE_GLTHREAD_BEGIN(base, curr){

        if(comp_fn(GLTHREAD_GET_USER_DATA_FROM_OFFSET(node, offset), 
                GLTHREAD_GET_USER_DATA_FROM_OFFSET(curr, offset)) != -1){
            prev = curr;
            continue;
        }

        glthread_add_next(curr, node);
        return;

    }ITERATE_GLTHREAD_END(base, curr);

    /*Add in the end*/
    glthread_add_next(prev, node);
}
