#pragma once

#include "asset.h"
#include "texture.h"
#include <cstddef>

class Renderer {
private:
  size_t j = 0;
  double width;
  double height;
  std::vector<Asset> assets;

  void apply_asset(const Asset &asset, Texture2D &tex);

public:
  Texture2D main_tex;

  Renderer() = delete;
  Renderer(double width, double height);
  ~Renderer();

  void setup();
  void update_physics(double dt);
  void update();
  void draw();

  /* TODO:
   *  - void set_projection_matrix()
   *  - void apply_zoom()
   *  _ void translate()
   *
   * Basically transition to using matrix/linear math.
   */
};
