#include "noise.hpp"

vec2 randomGradient(int ix, int iy)
{
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2; 
	unsigned a = ix, b = iy;
	
	a *= 3284157443;
	b ^= a << s | a >> (w - s);
	b *= 1911520717;
	a ^= b << s | b >> (w - s);
	a *= 2048419325;
	float random = a * (f_PI / ~(~0u >> 1));
	vec2 v;
	v.x = cosf(random);
	v.y = sinf(random);
	
	return v;
}

float dotGridGradient(int ix, int iy, float x, float y)
{
	vec2 gradient = randomGradient(ix, iy);

	float dx = x - static_cast<float>(ix);
	float dy = y - static_cast<float>(iy);
	return (dx * gradient.x + dy * gradient.y);
}

float interpolate(float a0, float a1, float w)
{
	return (a1 - a0) * (3.0f - w * 2.0f) * w * w + a0;
}

float perlin(float x, float y)
{
	int x0  = static_cast<int>(x);
	int y0  = static_cast<int>(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	float sx = x - static_cast<float>(x0);
	float sy = y - static_cast<float>(y0);

	float n0 = dotGridGradient(x0, y0, x, y);
	float n1 = dotGridGradient(x1, y0, x, y);
	float ix0 = interpolate(n0, n1, sx);

	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x, y);
	float ix1 = interpolate(n0, n1, sx);

	return (interpolate(ix0, ix1, sy));
}
