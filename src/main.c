#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "fractal.h"

int main(int argc, char **argv) {
  App fractal;
  // Set up callbacks
  fractal.init = fractal_initialize;
  fractal.update = fractal_update;
  fractal.draw = fractal_draw;
  fractal.clean = fractal_terminate;
  // Set up default window settings
  fractal.width = 800;
  fractal.height = 600;
  fractal.title = "Fractal";
  fractal.fullscreen = false;
  // Set up state
  fractal.state = malloc(sizeof(FractalState));
  if (fractal.state == NULL) {
    return 2;
  }
  ((FractalState *)(fractal.state))->options.fp64 = false;
  ((FractalState *)(fractal.state))->options.julia = false;
  // Read command line arguments
  for (size_t i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--fp64") == 0 || strcmp(argv[i], "-64") == 0) {
      // Activate fp64 mode
      ((FractalState *)(fractal.state))->options.fp64 = true;
    } else if (strcmp(argv[i], "--fullscreen") == 0 ||\
      strcmp(argv[i], "-f") == 0) {
      // Activate fullscreen
      fractal.fullscreen = true;
    } else if (strcmp(argv[i], "--width") == 0 || strcmp(argv[i], "-w") == 0) {
      // Custom width
      if (i + 1 < argc) {
        fractal.width = atol(argv[i + 1]);
        i++;
      } else {
        fprintf(stderr, "Missing value for %s.\n", argv[i]);
        return 2;
      }
    } else if (strcmp(argv[i], "--height") == 0 ||\
      strcmp(argv[i], "-h") == 0) {
      // Custom height
      if (i + 1 < argc) {
        fractal.height = atol(argv[i + 1]);
        i++;
      } else {
        fprintf(stderr, "Missing value for %s.\n", argv[i]);
        return 2;
      }
    } else if (strcmp(argv[i], "--julia") == 0 ||\
      strcmp(argv[i], "-j") == 0) {
      // Activate julia mode
      ((FractalState *)(fractal.state))->options.julia = true;
    }
  }
  // Start the app
  app_start(&fractal);
  // Free fractal state
  free(fractal.state);
  return 0;
}
