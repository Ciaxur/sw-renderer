#include "compute.h"
#include <cmath>

namespace compute {
  float distance(const float x0, const float y0, const float x1, const float y1) {
    float a = x1 - x0;
    float b = y1 - y0;
    return (a * a) + (b * b);
  }

  bool boundary_check(const vec2 &p, const float x_max, const float y_max) {
    return !(p.x < 0 || p.x >= x_max || p.y < 0 || p.y >= y_max);
  }

  /* TODO: optimize by moving to matrix rotation */
  vec2 rotate_vec2(const vec2 &origin, const vec2 &v, const float deg) {
    const float s = std::sin(deg);
    const float c = std::cos(deg);

    /* Translates vector by origin back to 0,0 */
    const float x1 = v.x - origin.x;
    const float y1 = v.y - origin.y;

    /* Apply rotation */
    const float x2 = (x1 * c) - (y1 * s);
    const float y2 = (y1 * c) + (x1 * s);

    /* Translate back to origin */
    return {
        .x = origin.x + x2,
        .y = origin.y + y2,
    };
  }
}

