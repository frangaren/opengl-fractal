#include "fractal.h"

#include <stdio.h>
#include <stdlib.h>

static GLuint fractal_create_vao();
static GLuint fractal_create_vbo();
static GLuint fractal_load_shader(GLenum type, const char *path);
static bool fractal_create_shader_program(FractalState *state);

bool fractal_initialize(App *app) {
  FractalState *s = app->state;
  s->vao = fractal_create_vao();
  s->vbo = fractal_create_vbo();
  s->vbo_size = 4;
  s->vertex_shader = fractal_load_shader(GL_VERTEX_SHADER,\
    "vertex_shader.glsl");
  if (s->vertex_shader == 0) {
    return false;
  }
  s->fragment_shader = fractal_load_shader(GL_FRAGMENT_SHADER,\
    "fragment_shader.glsl");
  if (s->fragment_shader == 0) {
    return false;
  }
  if (!fractal_create_shader_program(s)) {
    return false;
  }
  s->uniform_resolution = glGetUniformLocation(s->shader_program,"resolution");
  glUniform2f(s->uniform_resolution, (float)(app->width),(float)(app->height));
  s->uniform_offset = glGetUniformLocation(s->shader_program, "offset");
  glUniform2f(s->uniform_offset, 0.0f, 0.0f);
  s->uniform_zoom = glGetUniformLocation(s->shader_program, "zoom");
  glUniform1f(s->uniform_zoom, 1.0f);
  return true;
}

static GLuint fractal_create_vao() {
  GLuint vao;
  // Create VAO
  glGenVertexArrays(1, &vao);
  // Bind VAO
  glBindVertexArray(vao);
  return vao;
}

static GLuint fractal_create_vbo() {
  GLuint vbo;
  // Create VBO
  glGenBuffers(1, &vbo);
  // Bind VBO
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Populate VBO
  float vertices[] = {
    -1.0,  1.0,
     1.0,  1.0,
    -1.0, -1.0,
     1.0, -1.0
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // Set VBO format
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
  glEnableVertexAttribArray(0);
  return vbo;
}

static GLuint fractal_load_shader(GLenum type, const char *path) {
  // Open the shader file.
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Can't open file %s.\n", path);
    return 0;
  }
  // Get file size in bytes.
  fseek(file, 0, SEEK_END);
  int file_size = (int)ftell(file);
  // Allocate a large enough buffer.
  char * const source = malloc(file_size);
  if (source == NULL) {
    fprintf(stderr, "Can't allocate enough memory to hold %s.\n", path);
    fclose(file);
    return 0;
  }
  // Fill the buffer with the data in the file.
  fseek(file, 0, SEEK_SET);
  fread(source, file_size, 1, file);
  // Close the file.
  fclose(file);
  file = NULL;
  // Create the shader.
  GLuint shader = glCreateShader(type);
  if (shader == 0) {
    fprintf(stderr, "Can't create shader.\n");
    free(source);
    return 0;
  }
  // Load the source in the shader.
  glShaderSource(shader, 1, (const char * const *)&source, &file_size);
  // Compile the shader.
  glCompileShader(shader);
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    char buffer[512];
    glGetShaderInfoLog(shader, 512, NULL, buffer);
    fprintf(stderr, "Can't compile shader %s: %s\n", path, buffer);
    free(source);
    return 0;
  }
  // Free the source code.
  free(source);
  return shader;
}

static bool fractal_create_shader_program(FractalState *s) {
  // Create program
  s->shader_program = glCreateProgram();
  glAttachShader(s->shader_program, s->vertex_shader);
  glAttachShader(s->shader_program, s->fragment_shader);
  // Link program
  glLinkProgram(s->shader_program);
  GLint status;
  glGetProgramiv(s->shader_program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    char buffer[512];
    glGetProgramInfoLog(s->shader_program, 512, NULL, buffer);
    fprintf(stderr, "Can't link shader program: %s", buffer);
    return false;
  }
  // Bind program
  glUseProgram(s->shader_program);
  return true;
}

bool fractal_update(App *app) {
  //FractalState *s = app->state;
  return true;
}

bool fractal_draw(App *app) {
  FractalState *s = app->state;
  glBindVertexArray(s->vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, s->vbo_size);
  return true;
}

void fractal_terminate(App *app) {
  //FractalState *s = app->state;
  return;
}
