#pragma once

#include <cmath>
#include <vector>
#include "flowfield.hpp"
#include "math_types.hpp"

struct pdsContext
{
	std::vector<int>	grid;
	std::vector<int>	activeList;
	float			cellSize;
	int			cols;
	int			rows;
};

void	poissonDiskSampling(std::vector<vec2>& outputList, const otData& data);