#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct app {
  // Functions
  bool (*init)(struct app *app);
  bool (*update)(struct app *app);
  bool (*draw)(struct app *app);
  void (*clean)(struct app *app);
  // State
  void *state;
  // Window settings
  size_t width;
  size_t height;
  const char *title;
  bool fullscreen;
  GLFWwindow *window;
} App;

typedef bool (*AppInitFunc)(App *app);
typedef bool (*AppUpdateFunc)(App *app);
typedef bool (*AppDrawFunc)(App *app);
typedef void (*AppCleanFunc)(App *app);

bool app_start(App *app);
