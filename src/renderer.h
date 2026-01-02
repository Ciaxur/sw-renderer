#pragma once

#include "texture.h"
#include <cstddef>

class Renderer {
private:
  size_t j = 0;
  double width;
  double height;

public:
  Texture2D main_tex;

public:
  Renderer() = delete;
  Renderer(double width, double height);
  ~Renderer();

  void setup();
  void update_physics(double dt);
  void update();
  void draw();
};
