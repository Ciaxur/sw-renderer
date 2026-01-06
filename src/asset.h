#pragma once

#include <cstdint>
#include <vector>
#include "vec.h"

class Asset {
private:
  std::vector<vec2> points;
  vec2 axis;

public:
  Asset();
  ~Asset();

  void add_vertex(const vec2 &vert);
  void set_axis(const vec2 &v);
  const std::vector<vec2> &data() const;
  const vec2& get_axis() const;

public:
  void rotate(const float deg);
  void translate(const vec2 &d);
  void scale(const vec2 &v);
};

namespace geometry {
Asset create_rectangle(const int64_t x, const int64_t y, const int64_t w, const int64_t h);
Asset create_circle(const int64_t x, const int64_t y, const int64_t r);
}
