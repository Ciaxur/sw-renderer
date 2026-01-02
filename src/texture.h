#pragma once
#include <GL/gl.h>
#include <cstddef>
#include <vector>

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

  /**
   * TEST: Used for testing purposes.
   * This populates the current texture's pixel data with a gradient.
   */
  void generate_gradient();

  Texture2D(size_t width, size_t height);
  ~Texture2D();

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
};
