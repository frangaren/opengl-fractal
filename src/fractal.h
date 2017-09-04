#pragma once

#include <GL/glew.h>
#include <stdbool.h>
#include <stddef.h>

#include "app.h"

typedef struct {
  GLuint vao;
  GLuint vbo;
  size_t vbo_size;
  GLuint vertex_shader;
  GLuint fragment_shader;
  GLuint shader_program;
  GLint uniform_resolution;
  GLint uniform_offset;
  GLint uniform_zoom;
} FractalState;

bool fractal_initialize(App *app);
bool fractal_update(App *app);
bool fractal_draw(App *app);
void fractal_terminate(App *app);
