#include "flowfield.hpp"

std::vector<vec2>	makeGrid(const meridianData& data)
{
	std::vector<vec2> grid(WIDTH * HEIGHT);

	float n, angle;

	for (int j = 0; j < HEIGHT; ++j)
	{
		for (int i = 0; i < WIDTH; ++i) 
		{
			n = perlin (i * data.scale, j * data.scale);
			angle = n * f_PI * 2.0f;//n * 2.0f * 3.14159f; for all directions
			grid[j * WIDTH + i] = {static_cast<float>(cosf(angle)), static_cast<float>(sinf(angle))};
		}
	}
	return (grid);
}

void	pushTriangles(vec2 currentP, vec2 nextP, std::vector<vec2>& allSegments, int index)
{
	vec2	v1, v2, v3, v4;
	vec2	vect_dirr, vdirr_normal;
	float	vdirr_len, t = 2.0f * sin(f_PI * index * 0.002);

	vect_dirr = {nextP.x - currentP.x, nextP.y - currentP.y};
	vdirr_len = sqrtf( vect_dirr.x * vect_dirr.x + vect_dirr.y * vect_dirr.y);
	if (vdirr_len < 0.0001f)
		return;

	vect_dirr = {vect_dirr.x / vdirr_len, vect_dirr.y / vdirr_len};
	vdirr_normal = { - vect_dirr.y, vect_dirr.x};

	v1 = {currentP.x + vdirr_normal.x * t, currentP.y + vdirr_normal.y * t};
	v2 = {currentP.x - vdirr_normal.x * t, currentP.y - vdirr_normal.y * t};
	v3 = {nextP.x + vdirr_normal.x * t, nextP.y + vdirr_normal.y * t};
	v4 = {nextP.x - vdirr_normal.x * t, nextP.y - vdirr_normal.y * t};

	allSegments.push_back(v1);
	allSegments.push_back(v2);
	allSegments.push_back(v3);
	allSegments.push_back(v2);
	allSegments.push_back(v4);
	allSegments.push_back(v3);
}

void	makeSegments(vec2 start, const meridianData& data, const std::vector<vec2>& grid, std::vector<vec2>& allSegments)
{
	vec2 v, nextP, currentP = start;
	int x, y, i = -1;

	while (++i < 1000)
	{
		if (currentP.x <= 0 || currentP.x >= WIDTH || currentP.y <= 0 || currentP.y >= HEIGHT)
			break;

		x = static_cast<int>(currentP.x);
		y = static_cast<int>(currentP.y);
		v = grid[y * WIDTH + x];
		nextP = {currentP.x + v.x * data.stepSize, currentP.y + v.y * data.stepSize};

		pushTriangles(currentP, nextP, allSegments, i);

		currentP = nextP;
	}
}
