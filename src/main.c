#include <stdlib.h>

#include "app.h"

typedef struct {
} FractalState;

static bool fractal_initialize(void *state);
static bool fractal_update(void *state);
static bool fractal_draw(void *state);
static void fractal_terminate(void *state);

int main(int argc, char **argv) {
  App fractal;
  // Set up callbacks
  fractal.init = fractal_initialize;
  fractal.update = fractal_update;
  fractal.draw = fractal_draw;
  fractal.clean = fractal_terminate;
  // Set up state
  fractal.state = malloc(sizeof(FractalState));
  if (fractal.state == NULL) {
    return 2;
  }
  // Set up window settings
  fractal.width = 800;
  fractal.height = 600;
  fractal.title = "Fractal";
  fractal.fullscreen = false;
  // Start the app
  app_start(&fractal);
  // Free fractal state
  free(fractal.state);
  return 0;
}

static bool fractal_initialize(void *state) {
  return true;
}

static bool fractal_update(void *state) {
  return true;
}

static bool fractal_draw(void *state) {
  return true;
}

static void fractal_terminate(void *state) {
  return;
}
