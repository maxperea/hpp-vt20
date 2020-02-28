#include "galsim.h"
#include "quad_tree.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


void update_mass(quad_node_t *qnode, particle *body){
  if (qnode->mass == body->mass && qnode->com[0] == body->x[0] && qnode->com[1] == body->x[1])
    return; //This is to avoid updating twice for bodies being pushed down in the tree.
  qnode->com[0] += body->mass*body->x[0];
  qnode->com[1] += body->mass*body->x[1];
  
  //double new_x = (qnode->mass*qnode->com[0] + body->mass*body->x[0])/(qnode->mass + body->mass);
  //double new_y = (qnode->mass*qnode->com[1] + body->mass*body->x[1])/(qnode->mass + body->mass);
  //qnode->com[0] = new_x;
  //qnode->com[1] = new_y;
  qnode->mass += body->mass;
}

quad_node_t *new_quad_node(){
  return calloc(1, sizeof(quad_node_t));
}


void insert_aux(quad_node_t *qnode, particle *body, double x, double y, double side_length){
  //print_tree(qnode, 1);
  if (qnode->mass == 0){ // Basecase, om det är en tom lövnod -> placera body i noden

    qnode->body = body;
    qnode->mass = body->mass;
    qnode->com[0] = body->x[0];
    qnode->com[1] = body->x[1];

  } else if (qnode->body == NULL){ //Om det är en mittennod --> anropa rekursivt på rätt kvadrant

    //update_mass(qnode, body);
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

void update_com(quad_node_t *root)
{

  if (root && root->mass > 0){
    update_com(root->nw_child);
    update_com(root->ne_child);
    update_com(root->sw_child);
    update_com(root->se_child);

    double comx = 0;
    double comy = 0;
    root->mass = 0;

    if(root->nw_child)
      {
        root->mass += root->nw_child->mass;
        comx += root->nw_child->com[0]*root->nw_child->mass;
        comy += root->nw_child->com[1]*root->nw_child->mass;
      }
    if(root->ne_child)
      {
        root->mass += root->ne_child->mass;
        comx += root->ne_child->com[0]*root->ne_child->mass;
        comy += root->ne_child->com[1]*root->ne_child->mass;
      }
    if(root->sw_child)
      {
        root->mass += root->sw_child->mass;
        comx += root->sw_child->com[0]*root->sw_child->mass;
        comy += root->sw_child->com[1]*root->sw_child->mass;
      }
    if(root->se_child)
      {
        root->mass += root->se_child->mass;
        comx += root->se_child->com[0]*root->se_child->mass;
        comy += root->se_child->com[1]*root->se_child->mass;
      }

    if(root->body)
      {
        root->mass = root->body->mass;
        root->com[0] = root->body->x[0];
        root->com[1] = root->body->x[1];
      }
    else
      {
        root->com[0] = comx/root->mass;
        root->com[1] = comy/root->mass;
      }
    //printf("root com x %f, root com y %f ---\n", root->com[0], root->com[1]);
    //printf("comx %f, comy %f ---\n", comx, comy);
  }
}

quad_node_t *construct_quad_tree(particle* data, int N){
  quad_node_t *root = new_quad_node();
  //print_tree(root,1);
  
  for(int i = 0; i < N; i++){
    insert(root, (data + i));
  }
  update_com(root);
  //print_tree(root, 1);
  return root;
}

void print_tree(quad_node_t *root, int layer)
{
  printf("LAYER %d\n", layer);
  if(root && root->body == NULL)
    {
      printf("Mass: %f, com: %f,%f\n", root->mass, root->com[0],root->com[1]);
      print_tree(root->ne_child, layer+1);
      print_tree(root->nw_child, layer+1);
      print_tree(root->se_child, layer+1);
      print_tree(root->sw_child, layer+1);
    }
  else
    {
      printf("Node!\n-----\n");
    }
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
