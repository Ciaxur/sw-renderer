#include "texture.h"
#include <cmath>

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
