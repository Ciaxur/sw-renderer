#include "asset.h"
#include "compute.h"
#include <cmath>

Asset::Asset() : points({}) {}
Asset::~Asset() {}

void Asset::set_axis(const vec2 &v) { axis = v; }

void Asset::add_vertex(const vec2 &vert) { points.push_back(vert); }

const std::vector<vec2> &Asset::data() const { return points; }

const vec2 &Asset::get_axis() const { return axis; }

void Asset::rotate(const float deg) {
  const float s = std::sin(deg);
  const float c = std::cos(deg);

  for (vec2& p : points) {
    const float x = p.x - axis.x;
    const float y = p.y - axis.y;

    const float dx = axis.x + (x * c) - (y * s);
    const float dy = axis.y + (y * c) + (x * s);

    p.x = dx;
    p.y = dy;
  }
}

void Asset::translate(const vec2 &d) {
  axis.x += d.x;
  axis.y += d.y;

  for (vec2 &p : points) {
    p.x += d.x;
    p.y += d.y;
  }
}

void Asset::scale(const vec2 &v) {
  for (vec2 &p : points) {
    p.x = axis.x + (v.x * (p.x - axis.x));
    p.y = axis.y + (v.y * (p.y - axis.y));
  }
}

namespace geometry {
  Asset create_rectangle(const int64_t x, const int64_t y, const int64_t w, const int64_t h) {
    Asset asset;

    int64_t y_max = y + h;
    int64_t x_max = x + w;

    for (int64_t y_dt = y; y_dt < y_max; y_dt++) {
      for (int64_t x_dt = x; x_dt < x_max; x_dt++) {
        asset.add_vertex({static_cast<float>(x_dt), static_cast<float>(y_dt)});
      }
    }

    asset.set_axis({static_cast<float>(x), static_cast<float>(y)});
    return asset;
  }

  Asset create_circle(const int64_t x, const int64_t y, const int64_t r) {
    Asset asset;

    int64_t y_min = y - r;
    int64_t y_max = y + r;

    int64_t x_min = x - r;
    int64_t x_max = x + r;

    for (int64_t y_dt = y_min; y_dt < y_max; y_dt++) {
      for (int64_t x_dt = x_min; x_dt < x_max; x_dt++) {
        float d = compute::distance(x, y, x_dt, y_dt);
        if (d <= r * r) {
          asset.add_vertex({static_cast<float>(x_dt), static_cast<float>(y_dt)});
        }
      }
    }

    asset.set_axis({static_cast<float>(x), static_cast<float>(y)});
    return asset;
  }
}
