#include "galsim.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
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

void update_mass(quad_node_t *qnode, particle *body){
  if (qnode->mass == body->mass && qnode->com[0] == body->x[0] && qnode->com[1] == body->x[1])
    return; //This is to avoid updating twice for bodies being pushed down in the tree.
  double new_x = (qnode->mass*qnode->com[0] + body->mass*body->x[0])/(qnode->mass + body->mass);
  double new_y = (qnode->mass*qnode->com[1] + body->mass*body->x[1])/(qnode->mass + body->mass);
  qnode->com[0] = new_x;
  qnode->com[1] = new_y;
  qnode->mass = qnode->mass + body->mass;
}

quad_node_t *new_quad_node(){
  return calloc(1, sizeof(quad_node_t));
}

void insert_aux(quad_node_t *qnode, particle *body, double x, double y, double side_length){
  if (body->x[0] < x || body->x[0] > x + side_length || body->x[1] < y || body->x[1] > y + side_length)
    return;
  if (qnode->mass == 0){ // Basecase, om det är en tom lövnod -> placera body i noden
    qnode->body = body;
    qnode->mass = body->mass;
    qnode->com[0] = body->x[0];
    qnode->com[1] = body->x[1];
  } else if (qnode->body == NULL){ //Om det är en mittennod --> anropa rekursivt på rätt kvadrant
    update_mass(qnode, body);
    if (body->x[0] <= x + side_length/2 && body->x[1] <= y + side_length/2){
      insert_aux(qnode->nw_child, body, x, y, side_length/2);
    } else if (body->x[0] > x + side_length/2 && body->x[1] <= y + side_length/2){
      insert_aux(qnode->ne_child, body, x + side_length/2, y, side_length/2);
    } else if (body->x[0] <= x + side_length/2 && body->x[1] > y + side_length/2){
      insert_aux(qnode->sw_child, body, x, y + side_length/2, side_length/2);
    } else if (body->x[0] > x + side_length/2 && body->x[1] > y + side_length/2){
      insert_aux(qnode->se_child, body, x + side_length/2, y + side_length/2, side_length/2);
    }
  } else { // Om vi når en icke-tom lövnod -> splitta till fyra tomma lövnoder och anropa rekursivt med båda.
    qnode->nw_child = new_quad_node();
    qnode->ne_child = new_quad_node();
    qnode->sw_child = new_quad_node();
    qnode->se_child = new_quad_node();

    particle *tmp = qnode->body;
    qnode->body = NULL;

    insert_aux(qnode, tmp, x, y, side_length);
    insert_aux(qnode, body, x, y, side_length);
  }
}

void insert(quad_node_t *root, particle *body){
  insert_aux(root, body, 0, 0, 1);
}

quad_node_t *construct_quad_tree(particle* data, int N){
  quad_node_t *root = new_quad_node();
  for(int i = 0; i < N; i++){
    insert(root, (data + i));
  }
  return root;
}

void quad_tree_destroy(quad_node_t *root){
  if (root){
    quad_tree_destroy(root->nw_child);
    quad_tree_destroy(root->ne_child);
    quad_tree_destroy(root->sw_child);
    quad_tree_destroy(root->se_child);
    free(root);
  }
}
