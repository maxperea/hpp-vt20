#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "galsim.h"

typedef struct quad_node quad_node_t;

struct quad_node{
  particle *body;
  double mass;
  double com[2];
  quad_node_t *nw_child;
  quad_node_t *ne_child;
  quad_node_t *sw_child;
  quad_node_t *se_child;
};

quad_node_t *construct_quad_tree(particle* data, int N);

void quad_tree_destroy(quad_node_t *root);


#endif
