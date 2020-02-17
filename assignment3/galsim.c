#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_handler.h"
#include "graphics.h"

const double circle_radius=0.0025, circle_color=0;
const double e0 = 1e-3;
//const double dt = 1e-5;

void draw_particles(particle* data, int N)
{
  for(int i = 0; i < N; i++)
  {
    DrawCircle(*((data + i)->x), *((data + i)->x+1), 1, 1, (data + i)->mass*circle_radius, circle_color);
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
  int scalar = -(100/N) * data[i].mass;
  F[0] = scalar*F[0];
  F[1] = scalar*F[1];
}

// Uppdaterar partikel i enligt kraften F efter tidssteg dt.
void update_particle(double *F, particle *data, int i, double dt){
  double a[2];

  a[0] = F[0] / data[i].mass;
  a[1] = F[1] / data[i].mass;

  data[i].u[0] = data[i].u[0] + dt*a[0];
  data[i].u[1] = data[i].u[1] + dt*a[1];

  data[i].x[0] = data[i].x[0] + dt*data[i].u[0];
  data[i].x[1] = data[i].x[1] + dt*data[i].u[1];
}

// What to do for each step
// Alla ska väl uppdateras samtidigt, dvs för varje timestep
// Så hur uppdatera data? Kopia på data och sen byta pekare? dubbelpekare in?
void step(particle* data, double delta_t, int N)
{
  for(int i = 0; i < N; i++)
  {
    double F[2] = {0, 0};
    get_force(F, data, i, N);
    update_particle(F, data, i, delta_t);
  }
}

int main(int argc, char* argv[])
{
  // ./galsim N filename nsteps delta_t graphics

  //TODO check på dessa? Eller anta de är korrekta?
  int N = atoi(argv[1]);
  char* filename = argv[2];
  int steps = atoi(argv[3]);
  double delta_t = atof(argv[4]);
  int g = atoi(argv[5]);

  particle* data = read_file(filename, N);

  if(g)
  {
    int wdw_width = 800, wdw_height = 600;
    InitializeGraphics(argv[0], wdw_width, wdw_height);

    while(!CheckForQuit()) {
      ClearScreen();
      draw_particles(data, N);
      Refresh();
      if(steps > 0)
      {
        //printf("X1: %f  Y1: %f, X2: %f Y2: %f\n", data[0].x[0], data[0].x[1], data[1].x[0], data[1].x[1]);
        step(data, delta_t, N);
        steps--;
      }
    }
    FlushDisplay();
    CloseDisplay();
  }
  else
  {
    while(steps > 0){
      step(data, delta_t, N);
      steps--;
    }
  }

  //TODO write_file(filename, data, N);

  free(data);
}
