#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stddef.h>

typedef bool (*AppInitFunc)(void *state);
typedef bool (*AppUpdateFunc)(void *state);
typedef bool (*AppDrawFunc)(void *state);
typedef void (*AppCleanFunc)(void *state);

typedef struct {
  // Functions
  AppInitFunc init;
  AppUpdateFunc update;
  AppDrawFunc draw;
  AppCleanFunc clean;
  // State
  void *state;
  // Window settings
  size_t width;
  size_t height;
  const char *title;
  bool fullscreen;
  GLFWwindow *window;
} App;

bool app_start(App *app);
