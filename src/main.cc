#define RGFW_IMPLEMENTATION
#define RGFW_OPENGL
#include "RGFW.h"
#include <GL/gl.h>

#include <fmt/base.h>
#include <fmt/format.h>

void key_press(RGFW_window *window, RGFW_key key, u8 key_char,
               RGFW_keymod key_mod, RGFW_bool repeat, RGFW_bool pressed) {
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

int main() {
  RGFW_window *window = RGFW_createWindow(
      "Software Renderer", 0, 0, 800, 600,
      RGFW_windowCenter | RGFW_windowNoResize | RGFW_windowOpenGL);

  RGFW_setKeyCallback(key_press);

  while (RGFW_window_shouldClose(window) == RGFW_FALSE) {
    RGFW_pollEvents();

    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
      glColor3f(1.f, 0.f, 0.f); glVertex2f(-0.6f, -0.75f);
      glColor3f(0.f, 1.f, 0.f); glVertex2f(0.6f, -0.75f);
      glColor3f(0.f, 0.f, 1.f); glVertex2f(0.0f, 0.75f);
    glEnd();

    RGFW_window_swapBuffers_OpenGL(window);
    glFlush();
  }

  RGFW_window_close(window);
  return 0;
}

