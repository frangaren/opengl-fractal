#include "app.h"

#include <stdio.h>

static bool app_init(App *app);
static GLFWwindow *app_create_window(size_t width, size_t height,\
  const char *title, bool fullscreen);
static bool app_update(App *app);
static bool app_draw(App *app);
static void app_clean(App *app);
static void app_glfw_error_callback(int code, const char *description);

bool app_start(App *app) {
  if (!app_init(app)) {
    app_clean(app);
    return false;
  }
  while (!glfwWindowShouldClose(app->window)) {
    if (!app_update(app)) {
      app_clean(app);
      return false;
    }
    if (!app_draw(app)) {
      app_clean(app);
      return false;
    }
    glfwSwapBuffers(app->window);
    glfwPollEvents();
  }
  app_clean(app);
  return true;
}

static bool app_init(App *app) {
  // Initialize GLFW error callback
  glfwSetErrorCallback(app_glfw_error_callback);
  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "%s error: Couldn't initialize glfw.\n", __func__);
    return false;
  }
  // Create window
  app->window = app_create_window(app->width, app->height, app->title,\
    app->fullscreen);
  if (app->window == NULL) {
    fprintf(stderr, "%s error: Couldn't create window.\n", __func__);
    return false;
  }
  // Initialize GLEW
  GLenum glew_error = glewInit();
  if (glew_error != GLEW_OK) {
    fprintf(stderr, "%s error: Couldn't initialize glew.\n", __func__);
    fprintf(stderr, "glew error: %s.\n", glewGetErrorString(glew_error));
    return false;
  }
  // Call init callback
  if (!app->init(app)) {
    fprintf(stderr, "%s error: Init callback failed.\n", __func__);
    return false;
  }
  return true;
}

static GLFWwindow *app_create_window(size_t width, size_t height,\
  const char *title, bool fullscreen) {
  // OpenGL context parameters
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // Window parameters
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  // Window creation
  GLFWwindow *window = NULL;
  if (fullscreen) {
    window = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(),\
      NULL);
  } else {
    window = glfwCreateWindow(width, height, title, NULL, NULL);
  }
  glfwMakeContextCurrent(window);
  return window;
}

static bool app_update(App *app) {
  // Call update callback
  return app->update(app);
}

static bool app_draw(App *app) {
  // Call draw callback
  return app->draw(app);
}

static void app_clean(App *app) {
  // Call clean callback
  app->clean(app);
  // Terminate glfw
  glfwTerminate();
}

static void app_glfw_error_callback(int code, const char *description) {
  fprintf(stderr, "glfw error %d: %s\n", code, description);
}
