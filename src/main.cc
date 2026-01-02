#define RGFW_IMPLEMENTATION
#define RGFW_OPENGL
#include "RGFW.h"
#include <GL/gl.h>

#include <vector>
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

std::string get_opengl_version() {
  RGFW_glHints* opengl_hints = RGFW_getGlobalHints_OpenGL();
  return fmt::format("{}.{}", opengl_hints->major, opengl_hints->minor);
}

struct RGBA {
  GLubyte r;
  GLubyte g;
  GLubyte b;
  GLubyte a;

  void set(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
  }
};

struct Texture2D {
  GLuint id;
  size_t width;
  size_t height;
  std::vector<RGBA> pixels;

  // TEST: used for testing purposes.
  void generate_gradient() {
    float center_x = (float)width / 2.0f;
    float center_y = (float)height / 2.0f;
    float max_dist = sqrtf(center_x * center_x + center_y * center_y);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float dx = (float)x - center_x;
            float dy = (float)y - center_y;
            float distance = sqrtf(dx * dx + dy * dy);

            float dist_norm = distance / max_dist;

            GLubyte color_value = (GLubyte)(dist_norm * 255.0f);
            GLubyte inv_color_value = 255 - color_value;

            RGBA& p = at(x, y);
            p.set(inv_color_value, inv_color_value, color_value, 255);
        }
    }
  }

  Texture2D(size_t width, size_t height) : id(0), width(width), height(height), pixels(width * height) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    update();
  }

  ~Texture2D() {
    if (id) {
      glDeleteTextures(1, &id);
    }
  }

  RGBA &at(size_t x, size_t y) { return pixels[(y * width) + x]; }

  void update() {
    glBindTexture(GL_TEXTURE_2D, id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glBindTexture(GL_TEXTURE_2D, 0);
  }
};


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

double rgfw_get_time_s() {
  return RGFW_linux_getTimeNS(CLOCK_MONOTONIC) / 1000000000.0;
}

int main() {
  if (0){
    RGFW_glHints* opengl_hints = RGFW_getGlobalHints_OpenGL();
    opengl_hints->major = 4;
    opengl_hints->minor = 4;
    RGFW_setGlobalHints_OpenGL(opengl_hints);
  }

  const size_t WIDTH = 800;
  const size_t HEIGHT = 600;
  RGFW_window *window = RGFW_createWindow(
      "Software Renderer", 0, 0, WIDTH, HEIGHT,
      RGFW_windowCenter | RGFW_windowNoResize | RGFW_windowOpenGL);

  // glViewport(0, 0, WIDTH, HEIGHT);
  RGFW_window_makeCurrentContext_OpenGL(window);
  // glEnable(GL_DEPTH_TEST);

  RGFW_setKeyCallback(key_press);
  fmt::println("OpenGL Version: {}", get_opengl_version());

  Texture2D texture(WIDTH, HEIGHT);
  texture.generate_gradient();
  texture.update();


  /* FPS counter */
  uint64_t frame_count = 0;
  double last_time_s = rgfw_get_time_s();
  double fps = 0.0;

  // TEST:
  size_t j = 0;

  setup_2d_projection(WIDTH, HEIGHT);
  while (RGFW_window_shouldClose(window) == RGFW_FALSE) {
    RGFW_pollEvents();

    /* Measure the Speed (FPS) */
    double current_time = rgfw_get_time_s();
    frame_count++;
    if (current_time - last_time_s >= 1.0) {
      fps = frame_count;
      frame_count = 0;
      last_time_s += 1.0;
      fmt::println("FPS: {:.2f} | current_time = {:.2f}", fps, current_time);
    }

    /* Update state */
    // TEST:
    fmt::println("j={}", j);
    {
      for (size_t i = 0; i < texture.width; i++) {
        if (j > 0) {
          texture.at(i, j - 1).set(0, 0, 0, 255);
        }
        texture.at(i, j).set(255, 0, 0, 255);
      }

      j = (j+1) % texture.height;
      texture.update();
    }

    glClearColor(255.f, 255.f, 255.f, 255.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0.f, 0.f, 0.f);
    glScalef(1.f, 1.f, 0.f);

    glBindTexture(GL_TEXTURE_2D, texture.id);
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
    {
      /* Bottom-Left (0, 0) */
      glTexCoord2f(0.f, 0.f);
      glVertex3f(0.f, 0.f, 0.f);

      /* Top-Left (0, HEIGHT) */
      glTexCoord2f(0.f, 1.f);
      glVertex3f(0.f, (float)HEIGHT, 0.f);

      /* Top-Right (WIDTH, HEIGHT) */
      glTexCoord2f(1.f, 1.f);
      glVertex3f((float)WIDTH, (float)HEIGHT, 0.f);

      /* Bottom-Right (WIDTH, 0) */
      glTexCoord2f(1.f, 0.f);
      glVertex3f((float)WIDTH, 0.f, 0.f);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    RGFW_window_swapBuffers_OpenGL(window);
    glFlush();
  }

  RGFW_window_close(window);
  return 0;
}

