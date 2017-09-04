#pragma once

#include <GL/glew.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  GLuint vao;
  GLuint vbo;
  size_t vbo_size;
  GLuint vertex_shader;
  GLuint fragment_shader;
  GLuint shader_program;
} FractalState;

bool fractal_initialize(void *state);
bool fractal_update(void *state);
bool fractal_draw(void *state);
void fractal_terminate(void *state);
