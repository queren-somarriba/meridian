#include "poissonDiskSampling.hpp"

void initPdsContext(pdsContext& ctx, std::vector<vec2>& outputList)
{
	float x = (static_cast<float>(std::rand()) / RAND_MAX) * WIDTH;
	float y = (static_cast<float>(std::rand()) / RAND_MAX) * HEIGHT;
	
	outputList.push_back({x, y});
	ctx.activeList.push_back(0);

	ctx.cellSize = PSD_R / sqrtf(2.0f);
	ctx.cols = std::ceil(WIDTH / ctx.cellSize);
	ctx.rows = std::ceil(HEIGHT / ctx.cellSize);
	ctx.grid = std::vector<int>(ctx.cols * ctx.rows, -1);
	
	int gridX = static_cast<int>(x / ctx.cellSize);
	int gridY = static_cast<int>(y / ctx.cellSize);
	ctx.grid[gridY * ctx.cols + gridX] = 0;
}

vec2	generateRandomCandidate(const vec2& ref_point)
{
	float	angle, len, x, y;

	angle = 2.0f * f_PI * (static_cast<float>(std::rand()) / RAND_MAX);
	len = PSD_R + (static_cast<float>(std::rand()) / RAND_MAX) * PSD_R;
	x = len * cosf(angle) + ref_point.x;
	y = len * sinf(angle) + ref_point.y;

	return {x, y};
}

bool 	isCandidateValid(vec2 candidate, pdsContext& ctx, const std::vector<vec2>& outputList)
{
	if (candidate.x <= 0 || candidate.x >= WIDTH || candidate.y <= 0 || candidate.y >= HEIGHT)
		return false;

	int neighborID, cX, cY;
	float dx, dy;

	cX = static_cast<int>(candidate.x / ctx.cellSize);
	cY = static_cast<int>(candidate.y / ctx.cellSize);

	for (int j = -2; j <= 2; ++j)
	{
		for (int k = -2; k <= 2; ++k)
		{
			if (cX + j >= 0 && cX + j < ctx.cols && cY + k >= 0 && cY + k < ctx.rows)
			{
				neighborID = ctx.grid[(cY + k) * ctx.cols + cX + j];//select a cell near candidate

				if (neighborID != -1)//check if cell is empty
				{
					dx = candidate.x - outputList[neighborID].x;
					dy = candidate.y - outputList[neighborID].y;

					if (dx * dx + dy * dy < PSD_R * PSD_R)//check if the point in the cell is to close to the candidte 
						return false;
				}
			}
		}
	}

	return true;
}

void	poissonDiskSampling(std::vector<vec2>& outputList)
{
	pdsContext	ctx;
	initPdsContext(ctx, outputList);

	int	index, ref_point, new_id, cX, cY;
	vec2	candidate;
	bool	found;

	while (ctx.activeList.size())
	{
		index = std::rand() % ctx.activeList.size();
		ref_point = ctx.activeList[index];//random selection of a point that "touch" empty space
		found = false;

		for (int i = 0; i < 30; ++i)//generate 30 candidates points near ref_point
		{
			candidate = generateRandomCandidate(outputList[ref_point]);

			if (isCandidateValid(candidate, ctx, outputList))//check if a candidate is on an empty space
			{
				outputList.push_back(candidate);
				new_id = outputList.size() - 1;
				
				cX = static_cast<int>(candidate.x / ctx.cellSize);
				cY = static_cast<int>(candidate.y / ctx.cellSize);
				ctx.grid[cY * ctx.cols + cX] = new_id;
				
				ctx.activeList.push_back(new_id);
				found = true;
				break;
			}
		}
		if (!found)//if no candidate can be founc erase ref_point of activeList
		{
			ctx.activeList[index] = ctx.activeList.back();
			ctx.activeList.pop_back();
		}
	}
}