#include "flowfield.hpp"
#include "noise.hpp"

std::vector<vec2>	makeGrid(const otData& data)
{
	std::vector<vec2> grid(data.width * data.height);

	float n, angle;

	for (int j = 0; j < data.height; ++j)
	{
		for (int i = 0; i < data.width; ++i) 
		{
			n = perlin (i * data.scale + data.offsetX, j * data.scale + data.offsetY);
			angle = n * f_PI * 2.0f;
			grid[j * data.width + i] = {static_cast<float>(cosf(angle)), static_cast<float>(sinf(angle))};
		}
	}
	return (grid);
}

void	pushTriangles(vec2 currentP, vec2 nextP, std::vector<vec2>& allSegments, float t)
{
	vec2	v1, v2, v3, v4;
	vec2	vect_dirr, vdirr_normal;
	float	vdirr_len;

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

bool	checkCollision(vec2 nextP, int currentLine, collisionContext& col_ctx, const otData& data)
{
	if (nextP.x <= 0 || nextP.x >= data.width || nextP.y <= 0 || nextP.y >= data.height)
		return true;

	int neighborID, cX, cY;

	cX = static_cast<int>(nextP.x / col_ctx.cellSize);
	cY = static_cast<int>(nextP.y / col_ctx.cellSize);

	for (int j = -2; j <= 2; ++j)
	{
		for (int k = -2; k <= 2; ++k)
		{
			if (cX + j >= 0 && cX + j < col_ctx.cols && cY + k >= 0 && cY + k < col_ctx.rows)
			{
				neighborID = col_ctx.grid[(cY + k) * col_ctx.cols + cX + j];//select a cell near nextP

				if (neighborID != -1 && neighborID != currentLine)//check if cell is empty
					return true;
			}
		}
	}

	col_ctx.grid[cY * col_ctx.cols + cX] = currentLine;
	return false;
}

void	makeSegments(vec2 start, const otData& data, const std::vector<vec2>& grid,
		std::vector<vec2>& allSegments, int currentLine, collisionContext& col_ctx)
{
	std::vector<vec2> path;
	path.push_back(start);

	vec2 v, nextP, currentP = start;
	int x, y, i = -1;

	while (++i < MAX_ITERATIONS)
	{
		x = static_cast<int>(currentP.x);
		y = static_cast<int>(currentP.y);
		v = grid[y * data.width + x];
		nextP = {currentP.x + v.x * data.stepSize, currentP.y + v.y * data.stepSize};

		if (checkCollision(nextP, currentLine, col_ctx, data))
			break;

		path.push_back(nextP);
		currentP = nextP;
	}

	float ratio, t;

	if (path.size() > 2)
	{
		for (size_t j = 0; j < path.size() -1; ++j)
		{
			ratio = static_cast<float>(j) / static_cast<float>(path.size() - 1);
			t = std::sinf(f_PI * ratio) * 2.0f;
			pushTriangles(path[j], path[j + 1], allSegments, t);
		}
	}
}
