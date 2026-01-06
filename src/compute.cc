#include "compute.h"

namespace compute {
  float distance(const float x0, const float y0, const float x1, const float y1) {
    float a = x1 - x0;
    float b = y1 - y0;
    return (a * a) + (b * b);
  }

  bool boundary_check(const vec2 &p, const float x_max, const float y_max) {
    return !(p.x < 0 || p.x >= x_max || p.y < 0 || p.y >= y_max);
  }
}

