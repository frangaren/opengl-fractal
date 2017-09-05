#include "fractal.h"

#include <stdio.h>
#include <stdlib.h>

static App *fractal;

static GLuint fractal_create_vao();
static GLuint fractal_create_vbo();
static GLuint fractal_load_shader(GLenum type, const char *path);
static bool fractal_create_shader_program(FractalState *state);
static void fractal_update_resolution_uniform(App *app);
static void fractal_update_offset_uniform(FractalState *state);
static void fractal_update_zoom_uniform(FractalState *state);
static void fractal_update_julia_uniform(FractalState *state);
static void fractal_key_callback(GLFWwindow *window, int key, int scancode,\
  int action, int mods);
static void fractal_resize_callback(GLFWwindow *window, int width, int height);


bool fractal_initialize(App *app) {
  fractal = app;
  FractalState *s = app->state;
  if (s->options.fp64) {
    if (GLEW_ARB_gpu_shader_fp64) {
      fprintf(stderr, "Found extension ARB_gpu_shader_fp64. "\
        "Using 64 bits floating point shaders.\n");
    } else {
      fprintf(stderr, "Couldn't find extension ARB_gpu_shader_fp64. "\
        "Using 32 bits floating point shaders.\n");
      s->options.fp64 = false;
    }
  }
  s->vao = fractal_create_vao();
  s->vbo = fractal_create_vbo();
  s->vbo_size = 4;
  if (!fractal_create_shader_program(s)) {
    return false;
  }
  s->uniform_resolution = glGetUniformLocation(s->shader_program,"resolution");
  fractal_update_resolution_uniform(app);
  s->uniform_offset = glGetUniformLocation(s->shader_program, "offset");
  s->offset_x = 0.0;
  s->offset_y = 0.0;
  fractal_update_offset_uniform(s);
  s->uniform_zoom = glGetUniformLocation(s->shader_program, "zoom");
  s->zoom = 1.0;
  fractal_update_zoom_uniform(s);
  if (s->options.julia) {
    s->uniform_julia = glGetUniformLocation(s->shader_program, "jc");
    s->julia_x = 0.0;
    s->julia_y = 0.0;
    fractal_update_julia_uniform(s);
  }
  glfwSetWindowSizeCallback(app->window, fractal_resize_callback);
  glfwSetKeyCallback(app->window, fractal_key_callback);
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
  // Load vertex shader
  s->vertex_shader = fractal_load_shader(GL_VERTEX_SHADER,\
    "vertex_shader.glsl");
  if (s->vertex_shader == 0) {
    return false;
  }
  // Load fragment shader
  if (s->options.julia) {
    if (s->options.fp64) {
      s->fragment_shader = fractal_load_shader(GL_FRAGMENT_SHADER,\
        "julia_shader_fp64.glsl");
      if (s->fragment_shader == 0) {
        return false;
      }
    } else {
      s->fragment_shader = fractal_load_shader(GL_FRAGMENT_SHADER,\
        "julia_shader.glsl");
      if (s->fragment_shader == 0) {
        return false;
      }
    }
  } else {
    if (s->options.fp64) {
      s->fragment_shader = fractal_load_shader(GL_FRAGMENT_SHADER,\
        "mandelbrot_shader_fp64.glsl");
      if (s->fragment_shader == 0) {
        return false;
      }
    } else {
      s->fragment_shader = fractal_load_shader(GL_FRAGMENT_SHADER,\
        "mandelbrot_shader.glsl");
      if (s->fragment_shader == 0) {
        return false;
      }
    }
  }
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

static void fractal_update_resolution_uniform(App *app) {
  FractalState *s = fractal->state;
  if (s->options.fp64) {
    glUniform2d(s->uniform_resolution, app->width, app->height);
  } else {
    glUniform2f(s->uniform_resolution, app->width, app->height);
  }
}

static void fractal_update_offset_uniform(FractalState *s) {
  if (s->options.fp64) {
    glUniform2d(s->uniform_offset, s->offset_x, s->offset_y);
  } else {
    glUniform2f(s->uniform_offset, s->offset_x, s->offset_y);
  }
}

static void fractal_update_zoom_uniform(FractalState *s) {
  if (s->options.fp64) {
    glUniform1d(s->uniform_zoom, s->zoom);
  } else {
    glUniform1f(s->uniform_zoom, s->zoom);
  }
}

static void fractal_update_julia_uniform(FractalState *s) {
  if (s->options.julia) {
    if (s->options.fp64) {
      glUniform2d(s->uniform_julia, s->julia_x, s->julia_y);
    } else {
      glUniform2f(s->uniform_julia, s->julia_x, s->julia_y);
    }
  }
}

static void fractal_key_callback(GLFWwindow *window, int key, int scancode,\
  int action, int mods) {
  FractalState *s = fractal->state;
  switch (key) {
    case GLFW_KEY_W:
    case GLFW_KEY_UP:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        s->offset_y += 0.05*s->zoom;
        fractal_update_offset_uniform(s);
      }
      break;
    case GLFW_KEY_S:
    case GLFW_KEY_DOWN:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        s->offset_y -= 0.05*s->zoom;
        fractal_update_offset_uniform(s);
      }
      break;
    case GLFW_KEY_A:
    case GLFW_KEY_LEFT:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        s->offset_x += 0.05*s->zoom;
        fractal_update_offset_uniform(s);
      }
      break;
    case GLFW_KEY_D:
    case GLFW_KEY_RIGHT:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        s->offset_x -= 0.05*s->zoom;
        fractal_update_offset_uniform(s);
      }
      break;
    case GLFW_KEY_I:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        s->julia_y += 0.05;
        fractal_update_julia_uniform(s);
      }
      break;
    case GLFW_KEY_K:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        s->julia_y -= 0.05;
        fractal_update_julia_uniform(s);
      }
      break;
    case GLFW_KEY_J:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        s->julia_x += 0.05;
        fractal_update_julia_uniform(s);
      }
      break;
    case GLFW_KEY_L:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        s->julia_x -= 0.05;
        fractal_update_julia_uniform(s);
      }
      break;
    case GLFW_KEY_R:
    case GLFW_KEY_LEFT_SHIFT:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        s->zoom *= 1.05;
        fractal_update_zoom_uniform(s);
      }
      break;
    case GLFW_KEY_F:
    case GLFW_KEY_LEFT_CONTROL:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        s->zoom /= 1.05;
        fractal_update_zoom_uniform(s);
      }
      break;
    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
      if (action == GLFW_PRESS) {
        glfwSetWindowShouldClose(fractal->window, GLFW_TRUE);
      }
      break;
  }
}

static void fractal_resize_callback(GLFWwindow *window, int width, int height){
  fractal->width = width;
  fractal->height = height;
  glViewport(0, 0, width, height);
  fractal_update_resolution_uniform(fractal);
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
