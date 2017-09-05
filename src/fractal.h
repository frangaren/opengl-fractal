#pragma once

#include <GL/glew.h>
#include <stdbool.h>
#include <stddef.h>

#include "app.h"

typedef struct {
  struct {
    bool fp64;
    bool julia;
  } options;
  GLuint vao;
  GLuint vbo;
  size_t vbo_size;
  GLuint vertex_shader;
  GLuint fragment_shader;
  GLuint shader_program;
  GLint uniform_resolution;
  double offset_x;
  double offset_y;
  GLint uniform_offset;
  double zoom;
  GLint uniform_zoom;
  double julia_x;
  double julia_y;
  GLint uniform_julia;
} FractalState;

bool fractal_initialize(App *app);
bool fractal_update(App *app);
bool fractal_draw(App *app);
void fractal_terminate(App *app);
