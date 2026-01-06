#pragma once

#include "vec.h"
namespace compute {
float distance(const float x0, const float y0, const float x1, const float y1);

/**
 * Checks if the given vector is within boundaries.
 *
 * @param p vec2 reference to point vector being checked
 * @param x_max x-axis max value
 * @param y_max y-axis max value
 *
 * @returns whether given point is within boundary
 */
bool boundary_check(const vec2 &p, const float x_max, const float y_max);
}; // namespace compute
