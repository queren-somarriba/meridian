#pragma once

#include <iostream>
#include <vector>
#include "noise.hpp"

std::vector<vec2>	makeGrid(const meridianData& data);

void			makeSegments(vec2 start, const meridianData& data,
		const std::vector<vec2>& grid, std::vector<vec2>& allSegments,
			int currentLine, collisionContext& col_ctx);