#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app.h"
#include "fractal.h"

static void main_show_error(const char *option);
static void main_show_help();

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
    } else if (strcmp(argv[i], "--width") == 0 ||\
      strcmp(argv[i], "-ww") == 0) {
      // Custom width
      if (i + 1 < argc) {
        fractal.width = atol(argv[i + 1]);
        i++;
      } else {
        fprintf(stderr, "Missing value for %s.\n", argv[i]);
        free(fractal.state);
        return 2;
      }
    } else if (strcmp(argv[i], "--height") == 0 ||\
      strcmp(argv[i], "-wh") == 0) {
      // Custom height
      if (i + 1 < argc) {
        fractal.height = atol(argv[i + 1]);
        i++;
      } else {
        fprintf(stderr, "Missing value for %s.\n", argv[i]);
        free(fractal.state);
        return 2;
      }
    } else if (strcmp(argv[i], "--julia") == 0 ||\
      strcmp(argv[i], "-j") == 0) {
      // Activate julia mode
      ((FractalState *)(fractal.state))->options.julia = true;
    } else if (strcmp(argv[i], "--help") == 0 ||\
      strcmp(argv[i], "-h") == 0) {
      // Show help
      main_show_help();
      free(fractal.state);
      return 0;
    } else {
      main_show_error(argv[i]);
      free(fractal.state);
      return 2;
    }
  }
  // Start the app
  app_start(&fractal);
  // Free fractal state
  free(fractal.state);
  return 0;
}

static void main_show_error(const char *option) {
  fprintf(stderr, "Invalid option '%s'. Try with --help or -h.\n", option);
}
static void main_show_help() {
  fprintf(stderr,\
  "Controls:\n"\
  "\t- w and up arrow:     move fractal up.\n"\
  "\t- s and down arrow:   move fractal down.\n"\
  "\t- a and left arrow:   move fractal left.\n"\
  "\t- d and right arrow:  move fractal right.\n"\
  "\t- r and left shift:   zoom out.\n"\
  "\t- f and left control: zoom in.\n"\
  "\t- i, j, k and l:      modify Julia's constant (only in julia fractal).\n"\
  "\n"\
  "Options:\n"\
  "\t-64/--f64: enable  64bits floating point if supported (demanding).\n"\
  "\t-f/--fullscreen:   enable fullscreen.\n"\
  "\t-ww/--width value:  change width to value.\n"\
  "\t-wh/--height value: change height to value.\n"\
  "\t-j/--julia:        display Julia's set instead of Mandelbrot's.\n"\
  "\t-h/--help:         shows this message.\n"
  );
}
