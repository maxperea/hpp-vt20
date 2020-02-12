#include <stdlib.h>
#include <stdio.h>
#include "file_handler.h"
#include "graphics.h"

const float circle_radius=0.025, circle_color=0;

void draw_particles(particle* data, int N)
{
  for(int i = 0; i < N; i++)
    {
      DrawCircle((data + i)->x, (data + i)->y, 1, 1, (data + i)->mass*circle_radius, circle_color);
    }
}

// What to do for each step
// Alla ska väl uppdateras samtidigt, dvs för varje timestep
// Så hur uppdatera data? Kopia på data och sen byta pekare? dubbelpekare in?
void step(particle* data, double delta_t, int N)
{
  
  for(int i = 0; i < N; i++)
    {
      // Do stuff
    }
}

int main(int argc, char* argv[])
{
  // ./galsim N filename nsteps delta_t graphics

  //TODO check på dessa? Eller anta de är korrekta?
  int N = atoi(argv[1]);
  char* filename = argv[2];
  int steps = atoi(argv[3]);
  int delta_t = atof(argv[4]);
  int g = atoi(argv[5]);

  particle* data = read_file(filename, N);
  
  if(g)
    {
      int wdw_width = 800, wdw_height = 600;
      InitializeGraphics(argv[0], wdw_width, wdw_height);

      while(!CheckForQuit()) {
        if(steps > 0)
          {
            step(data, delta_t, N);
            steps--;
          }
        draw_particles(data, N);
        Refresh();
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
