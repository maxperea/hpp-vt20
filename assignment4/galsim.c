#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_handler.h"
#include "graphics.h"
#include "quad_tree.h"

const double circle_radius=0.0025, circle_color=0, rect_color=0;
const double e0 = 1e-3;

void draw_quads_aux(quad_node_t *node, double x, double y, double side_length){
  if(node){
    DrawRectangle(x, y, 1, 1, side_length, side_length, rect_color);
    draw_quads_aux(node->nw_child, x, y, side_length/2);
    draw_quads_aux(node->ne_child, x + side_length/2, y, side_length/2);
    draw_quads_aux(node->sw_child, x, y + side_length/2, side_length/2);
    draw_quads_aux(node->se_child, x + side_length/2, y + side_length/2, side_length/2);
  } else {
    return;
  }
}

void draw_quads(quad_node_t *root){
  draw_quads_aux(root, 0, 0, 1);
}

void draw_particles(particle* data, int N)
{
  for(int i = 0; i < N; i++)
  {
    DrawCircle(*((data + i)->x), *((data + i)->x+1), 1, 1, circle_radius, circle_color);
  }
}

// Räknar ut totala kraftvektorn F på partikel i och lagrar i array F
// x och y offset på rectangle?
void get_force(quad_node_t *node, double *F, particle *data,
               int N, double max_theta, double side_len, double box_y, double box_x){

  double r_ij;
  // If this is an empty leaf, do nothing
  if(node == NULL || node->mass == 0) return;
  
  //case 1: external node != this node
  if(node->body != NULL)
    {
      if(node->body == data) return; // If we reach leaf and it's this node - do nothing
        
      // TODO special case när en extern nod är på samma plats som data
      
      r_ij = sqrt(pow(data->x[0] - node->com[0], 2) + pow(data->x[1] - node->com[1], 2));
      F[0] += (node->mass / pow(r_ij + e0, 3)) * (data->x[0] - node->com[0]);
      F[1] += (node->mass / pow(r_ij + e0, 3)) * (data->x[1] - node->com[1]);
      return;
    }

  //double dist_box_mid = sqrt(pow(data->x[0] - node->com[0], 2) + pow(data->x[1]-node->com[1], 2));
  double dist_box_mid = sqrt(pow(data->x[0]-box_x, 2) + pow(data->x[1]-box_y, 2));
  double theta = side_len / dist_box_mid;
  
  //case 2: theta <= max_theta 
  if(theta <= max_theta)
    {
      r_ij = sqrt(pow(data->x[0] - node->com[0], 2) + pow(data->x[1] - node->com[1], 2));
      F[0] += (node->mass / pow(r_ij + e0, 3)) * (data->x[0] - node->com[0]);
      F[1] += (node->mass / pow(r_ij + e0, 3)) * (data->x[1] - node->com[1]);
    }

  //case 3: add branches values to F
  else
    {
      double quad_side = side_len/2;
      get_force(node->nw_child, F, data, N, max_theta, quad_side, box_y+quad_side, box_x-quad_side);
      get_force(node->ne_child, F, data, N, max_theta, quad_side, box_y+quad_side, box_x+quad_side);
      get_force(node->sw_child, F, data, N, max_theta, quad_side, box_y-quad_side, box_x-quad_side);
      get_force(node->se_child, F, data, N, max_theta, quad_side, box_y-quad_side, box_x+quad_side);
    }
}

// Uppdaterar partikel i enligt kraften F efter tidssteg dt.
void update_particle(double *F, particle *data, double dt){
  double a[2];

  a[0] = F[0] / data->mass;
  a[1] = F[1] / data->mass;

  data->u[0] = data->u[0] + dt*a[0];
  data->u[1] = data->u[1] + dt*a[1];

  data->x[0] = data->x[0] + dt*data->u[0];
  data->x[1] = data->x[1] + dt*data->u[1];
}

// What to do for each step
//void get_force(quad_node_t *node, double *F, particle *data,
//int N, double max_theta, double side_len, double box_y, double box_x){
void step(quad_node_t* root, particle* data, double delta_t, double max_theta, int N)
{
  //print_tree(root, 1);

  for(int i = 0; i < N; i++)
    {
      double F[2] = {0, 0};
      get_force(root, F, (data + i), N, max_theta, 1, 0.5, 0.5);
      double scalar = -(100/N) * data->mass;
      F[0] = scalar*F[0];
      F[1] = scalar*F[1];
      //printf("force x: %f, force y: %f\n", F[0],F[1]);
      update_particle(F, (data +i), delta_t);
    }
}

void swap(particle **x, particle **y){
  void *tmp = *x;
  *x = *y;
  *y = tmp;
}

int main(int argc, char* argv[])
{

  // ./galsim N filename nsteps delta_t max_theta graphics
  
  int N = atoi(argv[1]);
  char* filename = argv[2];
  int steps = atoi(argv[3]);
  double delta_t = atof(argv[4]);
  double max_theta = atof(argv[5]);
  int g = atoi(argv[6]);

  particle* data = read_file(filename, N);
  //particle* last_data = read_file(filename, N);
  quad_node_t *root = NULL;
  
  if(g)
  {
    int wdw_width = 800, wdw_height = 600;
    InitializeGraphics(argv[0], wdw_width, wdw_height);

    while(!CheckForQuit()) {
      if(steps > 0)
      {
        root = construct_quad_tree(data, N);
        step(root, data, delta_t, max_theta, N);
        ClearScreen();
        //draw_quads(root);
        quad_tree_destroy(root);
        draw_particles(data, N);
        Refresh();
        steps--;
      }
    }
    FlushDisplay();
    CloseDisplay();
  }
  else
  {
    while(steps > 0){
       root = construct_quad_tree(data, N);
       step(root, data, delta_t, max_theta, N);
       quad_tree_destroy(root);
       steps--;
    }
  }
  write_file("result.gal", data, N);

  free(data);
}
