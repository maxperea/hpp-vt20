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

void get_force_aux(quad_node_t *node, double max_theta, double *F, particle *data, int i, double x, double y, double side_length){
  double r_ij = 0;
  if (!node || node->mass == 0)
    return;
  if (node->body){
    if (node->body == data+i)
      return;
    r_ij = sqrt(pow(data[i].x[0] - node->com[0], 2) + pow(data[i].x[1] - node->com[1], 2));
    F[0] += (node->mass / pow(r_ij + e0, 3)) * (data[i].x[0] - node->com[0]);
    F[1] += (node->mass / pow(r_ij + e0, 3)) * (data[i].x[1] - node->com[1]);
    return;
  }
  double theta = side_length/(sqrt(pow(data[i].x[0] - x + side_length/2, 2) + pow(data[i].x[1] - y + side_length/2, 2)));
  if (theta <= max_theta){
    r_ij = sqrt(pow(data[i].x[0] - node->com[0], 2) + pow(data[i].x[1] - node->com[1], 2));
    F[0] += (node->mass / pow(r_ij + e0, 3)) * (data[i].x[0] - node->com[0]);
    F[1] += (node->mass / pow(r_ij + e0, 3)) * (data[i].x[1] - node->com[1]);
    return;
  }
  get_force_aux(node->nw_child, max_theta, F, data, i, x, y, side_length/2);
  get_force_aux(node->ne_child, max_theta, F, data, i, x + side_length/2, y, side_length/2);
  get_force_aux(node->sw_child, max_theta, F, data, i, x, y + side_length/2, side_length/2);
  get_force_aux(node->se_child, max_theta, F, data, i, x + side_length/2, y + side_length/2, side_length/2);
}

void get_force(quad_node_t *root, double max_theta, double *F, particle *data, int i, int N){
  get_force_aux(root, max_theta, F, data, i, 0, 0, 1);
  double scalar = -(100/N) * data[i].mass;
  F[0] = scalar*F[0];
  F[1] = scalar*F[1];
}

// Uppdaterar partikel i enligt kraften F efter tidssteg dt.
void update_particle(double *F, particle *data, particle *last_data, int i, double dt){
  double a[2];

  a[0] = F[0] / last_data[i].mass;
  a[1] = F[1] / last_data[i].mass;

  data[i].u[0] = last_data[i].u[0] + dt*a[0];
  data[i].u[1] = last_data[i].u[1] + dt*a[1];

  data[i].x[0] = last_data[i].x[0] + dt*data[i].u[0];
  data[i].x[1] = last_data[i].x[1] + dt*data[i].u[1];
}

// What to do for each step
void step(quad_node_t *root, double max_theta, particle* data, particle *last_data, double delta_t, int N)
{
  for(int i = 0; i < N; i++)
  {
    double F[2] = {0, 0};
    get_force(root, max_theta, F, last_data, i, N);
    update_particle(F, data, last_data, i, delta_t);
  }
}

void swap(particle **x, particle **y){
  void *tmp = *x;
  *x = *y;
  *y = tmp;
}

int main(int argc, char* argv[])
{

  // ./galsim N filename nsteps delta_t graphics
  int N = atoi(argv[1]);
  char* filename = argv[2];
  int steps = atoi(argv[3]);
  double delta_t = atof(argv[4]);
  double max_theta = atof(argv[5]);
  int g = atoi(argv[6]);

  particle* last_data = read_file(filename, N);
  particle* data = read_file(filename, N);

  //quad_node_t *root = construct_quad_tree(data, N);

  if(g)
  {
    int wdw_width = 800, wdw_height = 600;
    InitializeGraphics(argv[0], wdw_width, wdw_height);

    while(!CheckForQuit()) {
      if(steps > 0)
      {
        quad_node_t *root = construct_quad_tree(last_data, N);
        step(root, max_theta, data, last_data, delta_t, N);
        swap(&data, &last_data);
        ClearScreen();
        draw_particles(data, N);
        //draw_quads(root);
        quad_tree_destroy(root);
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
      quad_node_t *root = construct_quad_tree(last_data, N);
      step(root, max_theta, data, last_data, delta_t, N);
      swap(&data, &last_data);
      quad_tree_destroy(root);
      steps--;
    }
  }
  write_file("result.gal", last_data, N);

  free(data);
  free(last_data);
}
