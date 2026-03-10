#pragma once

#include <iostream>
#include <vector>
#include "math_types.hpp"

constexpr int MAX_ITERATIONS = 10000;

struct otData
{
	float	scale;
	float	stepSize;
	float	offsetX;
	float	offsetY;
	float	line_padding;
	float	pds_radius;
	float	speed;
	int	width;
	int	height;
	bool	record;
};

struct collisionContext
{
	std::vector<int>	grid;
	float			cellSize;
	int			cols;
	int			rows;
};

std::vector<vec2>	makeGrid(const otData& data);

void			makeSegments(vec2 start, const otData& data,
				const std::vector<vec2>& grid, std::vector<vec2>& allSegments,
					int currentLine, collisionContext& col_ctx);

std::vector<vec2>	flowfield(const otData& data);