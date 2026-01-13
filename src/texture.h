#pragma once
#include "vec.h"
#include <GL/gl.h>
#include <cstddef>
#include <vector>

struct RGBA {
  GLubyte r;
  GLubyte g;
  GLubyte b;
  GLubyte a;

  void set(RGBA rgba) { *this = rgba; }

  void set(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
  }
};

class Texture2D {
private:
  GLuint id;
  std::vector<RGBA> pixels;
  size_t width;
  size_t height;

public:
  Texture2D(size_t width, size_t height);
  ~Texture2D();

  void clear(RGBA color);

  /**
   * @returns current texture's OpenGL id.
   */
  GLuint get_texture_id();

  /**
   * TEST: Used for testing purposes.
   * This populates the current texture's pixel data with a gradient.
   */
  void generate_gradient();

  /**
   * Helper function for retrieving pixel data at the given (x,y) location.
   *
   * @param x Column to grab in pixel data
   * @param y Row to grab in pixel data
   *
   * @returns RGBA pixel reference.
   */
  RGBA &at(size_t x, size_t y);

  /**
   * Updates GL texture with current pixel data.
   */
  void update();

  /**
   * Draws a line between two points.
   *
   * @param x0 first x point.
   * @param y0 first y point.
   * @param x1 second x point.
   * @param y1 second y point.
   */
  void draw_line(size_t x0, size_t y0, size_t x1, size_t y1);

  /**
   * Draws a white dot at given location.
   *
   * @param x x-axis to draw dot
   * @param y y-axis to draw dot
   * @param color color to fill dot with
   */
  void draw_dot(int64_t x, int64_t y, RGBA color = { 255, 255, 255, 255 });

  void apply_projection(const float rot_deg, const vec3 &v_tran);
};
