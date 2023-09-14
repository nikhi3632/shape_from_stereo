#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "quadtree.h"
#include "make_qtree.h"
#include "utils.h"

#define ABS(x) (((x) < 0) ? (-(x)) : (x))
#define NON_HOMOGENEOUS_NON_LEAF_GRAYSCALE_VALUE 256
/*
    returns whether or not all pixels in given field have the same grayscale.
    returns 256 if non -homogenous and the grayscale value otherwise. 
*/
int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width) {
    int grayscale_value = depth_map[x + (y * map_width)]; 
    for (int i = 0; i < section_width; i++){
        for (int j = 0; j < section_width; j++){
            if (depth_map[(y + j) * map_width + (x + i)] != grayscale_value){
                grayscale_value = NON_HOMOGENEOUS_NON_LEAF_GRAYSCALE_VALUE;
                break;
            }
        }
    }
    return grayscale_value;
}

//Determines whether a given qNode is a Leaf. 

bool is_leaf(unsigned char *depth_map, int map_width, int x, int y, int section_width){
    if (homogenous(depth_map, map_width, x, y, section_width) == NON_HOMOGENEOUS_NON_LEAF_GRAYSCALE_VALUE){
        return false;
    }
    return true; 
}

void to_quad_tree(unsigned char *depth_map, int map_width, qNode *this_node, int size, int x, int y){

    if (is_leaf(depth_map, map_width, x, y, size)){

        this_node->leaf = 1;
        this_node->size = size;
        this_node->gray_value = homogenous(depth_map, map_width, x, y, size);
        this_node->x = x;
        this_node->y = y;

        this_node->child_NW = NULL;
        this_node->child_NE = NULL;
        this_node->child_SE = NULL;
        this_node->child_SW = NULL;

    }
    else {

        this_node->leaf = 0;
        this_node->size = size;
        this_node->gray_value = NON_HOMOGENEOUS_NON_LEAF_GRAYSCALE_VALUE;
        this_node->x = x;
        this_node->y = y;

        qNode *child1 = malloc(sizeof(qNode));
        qNode *child2 = malloc(sizeof(qNode));
        qNode *child3 = malloc(sizeof(qNode));
        qNode *child4 = malloc(sizeof(qNode));

        if (child1 == NULL || child2 == NULL ||child3 == NULL ||child4 == NULL){
            allocation_failed(); 
        }

        this_node->child_NW = child1;
        this_node->child_NE = child2;
        this_node->child_SW = child3;
        this_node->child_SE = child4;

        int mid = size/2;

        to_quad_tree(depth_map, map_width, child1, size/2, x, y);
        to_quad_tree(depth_map, map_width, child2, size/2, x + mid, y);
        to_quad_tree(depth_map, map_width, child3, size/2, x, y + mid);
        to_quad_tree(depth_map, map_width, child4, size/2, x + mid, y + mid);
    }
}

//Converts a depth map to a quad tree. Returns a pointer to root qNode. 
//Calls recursive function to_quad_tree 
qNode *depth_to_quad(unsigned char *depth_map, int map_width) {
    qNode *root;
    root = malloc(sizeof(qNode));

    if (root == NULL){
        allocation_failed(); 
    }

    to_quad_tree(depth_map, map_width, root, map_width, 0, 0); 
    /* malloc: if pointer returned by malloc is NULL, call allocation_failed() */
    return root;
}

void free_qtree(qNode *qtree_node) {
    if(qtree_node) {
        if(!qtree_node->leaf){
            free_qtree(qtree_node->child_NW);
            free_qtree(qtree_node->child_NE);
            free_qtree(qtree_node->child_SE);
            free_qtree(qtree_node->child_SW);
        }
        free(qtree_node);
    }
}