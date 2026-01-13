#include "texture.h"
#include "compute.h"
#include <cmath>
#include <cstdint>
#include <fmt/base.h>

void Texture2D::generate_gradient() {
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

      RGBA &p = at(x, y);
      p.set(inv_color_value, inv_color_value, color_value, 255);
    }
  }
}

Texture2D::Texture2D(size_t width, size_t height) : id(0), width(width), height(height), pixels(width * height) {
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

Texture2D::~Texture2D() {
  if (id) {
    glDeleteTextures(1, &id);
  }
}

RGBA &Texture2D::at(size_t x, size_t y) { return pixels[(y * width) + x]; }

void Texture2D::update() {
  glBindTexture(GL_TEXTURE_2D, id);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
  glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture2D::get_texture_id() { return id; }

void Texture2D::clear(RGBA color) {
  for (auto &pixel : pixels) {
    pixel.set(color.r, color.g, color.b, color.a);
  }
}

void Texture2D::draw_line(size_t x0, size_t y0, size_t x1, size_t y1) {
  /* Place dots */
  at(x0, y0).set(255, 255, 255, 255);
  at(x1, y1).set(255, 255, 255, 255);

  /* Slope */
  int64_t dx = x1 - x0;
  int64_t dy = y1 - y0;

  int64_t x_direction = dx > 0 ? 1 : -1;
  int64_t y_direction = dy > 0 ? 1 : -1;

  if (dy == 0) { y_direction = 0; }
  if (dx == 0) { x_direction = 0; }

  /* Walk it */
  int64_t xf = x0;
  int64_t yf = y0;

  while (xf != x1 || yf != y1) {
    at(xf, yf).set(255, 255, 255, 255);
    yf += y_direction;
    xf += x_direction;
  }
}

void Texture2D::draw_dot(int64_t x, int64_t y, RGBA color) {
  if (compute::boundary_check({static_cast<float>(x), static_cast<float>(y)}, width, height)) {
    at(x, y).set(color);
  }
}

void Texture2D::apply_projection(const float rot_deg, const vec3 &v_tran) {
  const std::vector<RGBA> current_state = pixels;

  clear({0, 0, 0, 255});
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      const RGBA &cp = current_state[(y * width) + x];

      vec2 new_pos = compute::rotate_vec2({0.f, 0.f}, {(float)x, (float)y}, rot_deg);

      /*
       * Apply xyz translation, which includes "panning" on the x/y axis and zooming via z-axis.
       *   z-translation occurs from the 0,0 on the texture.
       */
      new_pos.x = (new_pos.x * v_tran.z) + v_tran.x;
      new_pos.y = (new_pos.y * v_tran.z) + v_tran.y;
      if (compute::boundary_check(new_pos, width, height)) {
        at(new_pos.x, new_pos.y).set(cp);
      }
    }
  }
}
