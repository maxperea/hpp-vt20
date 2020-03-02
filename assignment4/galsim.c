#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_handler.h"
#include "graphics.h"
#include "quad_tree.h"

const double circle_radius=0.0025, circle_color=0, rect_color=0;
const double e0 = 1e-3;

void draw_particles(particle* data, int N){
  for(int i = 0; i < N; i++){
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
    r_ij = sqrt((data[i].x[0] - node->com[0])*(data[i].x[0] - node->com[0]) + (data[i].x[1] - node->com[1])*(data[i].x[1] - node->com[1]));
    F[0] += (node->mass / ((r_ij + e0)*(r_ij + e0)*(r_ij + e0)))*(data[i].x[0] - node->com[0]);
    F[1] += (node->mass / ((r_ij + e0)*(r_ij + e0)*(r_ij + e0)))*(data[i].x[1] - node->com[1]);
    return;
  }
  const double theta = side_length/(sqrt((data[i].x[0] - x + side_length/2)*(data[i].x[0] - x + side_length/2) + (data[i].x[1] - y + side_length/2)*(data[i].x[1] - y + side_length/2)));
  if (theta <= max_theta){
    r_ij = sqrt((data[i].x[0] - node->com[0])*(data[i].x[0] - node->com[0]) + (data[i].x[1] - node->com[1])*(data[i].x[1] - node->com[1]));
    F[0] += (node->mass / ((r_ij + e0)*(r_ij + e0)*(r_ij + e0)))*(data[i].x[0] - node->com[0]);
    F[1] += (node->mass / ((r_ij + e0)*(r_ij + e0)*(r_ij + e0)))*(data[i].x[1] - node->com[1]);
    return;
  }
  get_force_aux(node->nw_child, max_theta, F, data, i, x, y, side_length/2);
  get_force_aux(node->ne_child, max_theta, F, data, i, x + side_length/2, y, side_length/2);
  get_force_aux(node->sw_child, max_theta, F, data, i, x, y + side_length/2, side_length/2);
  get_force_aux(node->se_child, max_theta, F, data, i, x + side_length/2, y + side_length/2, side_length/2);
}

void get_force(quad_node_t *root, double max_theta, double *F, particle *data, int i, int N){
  get_force_aux(root, max_theta, F, data, i, 0, 0, 1);
  const double scalar = -(100/ (double) N) * data[i].mass;
  F[0] = scalar*F[0];
  F[1] = scalar*F[1];
}

void step(quad_node_t *root, double max_theta, particle* data, double dt, int N)
{
  for(int i = 0; i < N; i++){
    double F[2] = {0, 0};
    get_force(root, max_theta, F, data, i, N);
    double a[2];
    a[0] = F[0] / data[i].mass;
    a[1] = F[1] / data[i].mass;
    data[i].u[0] = data[i].u[0] + dt*a[0];
    data[i].u[1] = data[i].u[1] + dt*a[1];
    data[i].x[0] = data[i].x[0] + dt*data[i].u[0];
    data[i].x[1] = data[i].x[1] + dt*data[i].u[1];
  }
}

int main(int argc, char* argv[])
{
  const int N = atof(argv[1]);
  char* filename = argv[2];
  int steps = atoi(argv[3]);
  const double delta_t = atof(argv[4]);
  const double max_theta = atof(argv[5]);
  const int g = atoi(argv[6]);
  particle* data = read_file(filename, N);
  if (g) {
    const int wdw_width = 800, wdw_height = 600;
    InitializeGraphics(argv[0], wdw_width, wdw_height);
    while (!CheckForQuit() && steps > 0) {
      quad_node_t *root = construct_quad_tree(data, N);
      step(root, max_theta, data, delta_t, N);
      ClearScreen();
      draw_particles(data, N);
      quad_tree_destroy(root);
      Refresh();
      steps--;
    }
    FlushDisplay();
    CloseDisplay();
  } else {
    while (steps > 0){
      quad_node_t *root = construct_quad_tree(data, N);
      step(root, max_theta, data, delta_t, N);
      quad_tree_destroy(root);
      steps--;
    }
  }
  write_file("result.gal", data, N);
  free(data);
}
