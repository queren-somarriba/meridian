#pragma once

#include <cmath>
#include "math_types.hpp"

vec2	randomGradient(int ix, int iy);

float	dotGridGradient(int ix, int iy, float x, float y);

float	interpolate(float a0, float a1, float w);

float	perlin(float x, float y);
