/*
    Quadtree Compression
*/

#ifndef MAKE_QTREE_H
#define MAKE_QTREE_H

/* Implement this in make_qtree.c */
int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width);

/* Implement this in make_qtree.c */
qNode *depth_to_quad(unsigned char *depth_map, int image_width);

/* Function that frees the quadtree created */
void free_qtree(qNode *qtree_root);

void to_quad_tree(unsigned char *depth_map, int map_width, qNode *this_node, int size, int x, int y);

#endif
