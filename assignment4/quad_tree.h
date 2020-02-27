#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "galsim.h"

typedef struct quad_node quad_node_t;
quad_node_t *construct_quad_tree(particle* data, int N);

#endif
