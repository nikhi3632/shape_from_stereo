/*
    Quadtree Compression
*/

#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdbool.h>
#include "utils.h"

/*
 * A struct that represents one quadtree node. LEAF is a boolean value to
 * denote whether this node is a leaf: 0 = non-leaf, 1 = leaf. SIZE refers to
 * the number of pixels in the width of the image represented by this node. X
 * and Y refer to the coordinates of the top-left most pixel of the section.
 * GRAY_VALUE is set to a value in 0-255 for leaves, but can be anything for
 * non-leaves. The four children pointers should only point to other nodes if
 * this node is not a leaf.
 */
typedef struct quadtree qNode;
struct quadtree {
    bool leaf;
    int size;
    int x;
    int y;
    int gray_value; // 0-255 for leaves; -1 for internal nodes.
    qNode *child_NW, *child_NE, *child_SE, *child_SW;
};

/* Helpful functions in debugging */
void print_qtree(qNode *qtree_root);

void print_compressed(qNode *qtree_root);

#endif
