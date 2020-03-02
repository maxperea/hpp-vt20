#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_handler.h"
#include "graphics.h"

const double circle_radius=0.0025, circle_color=0;
const double e0 = 1e-3;

void draw_particles(particle* data, int N)
{
  for(int i = 0; i < N; i++)
  {
    DrawCircle(*((data + i)->x), *((data + i)->x+1), 1, 1, circle_radius, circle_color);
  }
}

// Räknar ut totala kraftvektorn F på partikel i och lagrar i array F
void get_force(double *F, particle *data, int i, int N){
  double r_ij = 0;
  for (int j = 0; j < N; j++){
    if(i != j){
      r_ij = sqrt(pow(data[i].x[0] - data[j].x[0], 2) + pow(data[i].x[1] - data[j].x[1], 2));
      F[0] += (data[j].mass / pow(r_ij + e0, 3)) * (data[i].x[0] - data[j].x[0]);
      F[1] += (data[j].mass / pow(r_ij + e0, 3)) * (data[i].x[1] - data[j].x[1]);
    }
  }
  double scalar = -(100/ (double) N) * data[i].mass;
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
void step(particle* data, particle *last_data, double delta_t, int N)
{
  for(int i = 0; i < N; i++)
  {
    double F[2] = {0, 0};
    get_force(F, last_data, i, N);
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
  int g = atoi(argv[5]);

  particle* last_data = read_file(filename, N);
  particle* data = read_file(filename, N);

  if(g)
  {
    int wdw_width = 800, wdw_height = 600;
    InitializeGraphics(argv[0], wdw_width, wdw_height);

    while(!CheckForQuit()) {
      if(steps > 0)
      {
        step(data, last_data, delta_t, N);
        swap(&data, &last_data);
        ClearScreen();
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
      step(data, last_data, delta_t, N);
      swap(&data, &last_data);
      steps--;
    }
  }
  write_file("result.gal", last_data, N);

  free(data);
  free(last_data);
}
