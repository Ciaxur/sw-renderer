#define RGFW_IMPLEMENTATION
#define RGFW_OPENGL
#include "RGFW.h"
#include <GL/gl.h>

#include "renderer.h"
#include <fmt/base.h>
#include <fmt/format.h>

void key_press(RGFW_window *window, RGFW_key key, u8 key_char, RGFW_keymod key_mod, RGFW_bool repeat,
               RGFW_bool pressed) {
  RGFW_UNUSED(repeat);
  if (!pressed) {
    return;
  }

  switch (key) {
  case RGFW_q: {
    RGFW_window_setShouldClose(window, true);
    break;
  }
  }
}

std::string get_opengl_version() {
  RGFW_glHints *opengl_hints = RGFW_getGlobalHints_OpenGL();
  return fmt::format("{}.{}", opengl_hints->major, opengl_hints->minor);
}

void setup_2d_projection(const size_t width, const size_t height) {
  /* 1. Enter projection mode */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  /*
   2. Set orthographic projection This maps (0,0) to the bottom-left of the
   viewport and (width, height) to the top-right. The Z-range (e.g., -1 to 1)
   allows for depth ordering, though often unnecessary for 2D.
   */
  glOrtho(0.0, (double)width, 0.0, (double)height, -1.0, 1.0);

  /* 3. Return to model_view mode for drawing */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* 4. Update the Viewport */
  glViewport(0, 0, width, height);
}

inline double rgfw_get_time_s() { return RGFW_linux_getTimeNS(CLOCK_MONOTONIC) / 1000000000.0; }

void set_gl_version(i32 major, i32 minor) {
  RGFW_glHints *opengl_hints = RGFW_getGlobalHints_OpenGL();
  opengl_hints->major = major;
  opengl_hints->minor = minor;
  RGFW_setGlobalHints_OpenGL(opengl_hints);
}

int main() {
  const size_t WIDTH = 800;
  const size_t HEIGHT = 600;

  set_gl_version(2, 1);
  RGFW_window *window = RGFW_createWindow("Software Renderer", 0, 0, WIDTH, HEIGHT,
                                          RGFW_windowCenter | RGFW_windowNoResize | RGFW_windowOpenGL);

  RGFW_window_makeCurrentContext_OpenGL(window);

  RGFW_setKeyCallback(key_press);
  fmt::println("OpenGL Version: {}", get_opengl_version());

  Renderer renderer(WIDTH, HEIGHT);
  renderer.setup();

  /* FPS counter */
  uint64_t frame_count = 0;
  double last_time_s = rgfw_get_time_s();
  double last_draw_call_time_s = last_time_s;
  double fps = 0.0;

  setup_2d_projection(WIDTH, HEIGHT);
  while (RGFW_window_shouldClose(window) == RGFW_FALSE) {
    RGFW_pollEvents();

    /* Measure the Speed (FPS) */
    double current_time = rgfw_get_time_s();
    double time_dt = current_time - last_time_s;
    frame_count++;
    if (time_dt >= 1.0) {
      fps = frame_count;
      frame_count = 0;
      last_time_s += 1.0;
      fmt::println("FPS: {:.2f} | current_time = {:.2f}", fps, current_time);
    }

    /* Physics update */
    double physics_dt = current_time - last_draw_call_time_s;
    renderer.update_physics(physics_dt);
    last_draw_call_time_s = current_time;

    /* Update state */
    renderer.update();

    /* Draw internal renderer's state */
    renderer.draw();

    RGFW_window_swapBuffers_OpenGL(window);
    glFlush();
  }

  RGFW_window_close(window);
  return 0;
}
