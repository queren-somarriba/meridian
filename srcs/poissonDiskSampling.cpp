#include "poissonDiskSampling.hpp"

void	poissonDiskSampling(const meridiansData& data, std::vector<vec2>& outputList)
{
	float x = (static_cast<float>(std::rand()) / RAND_MAX) * data.width;
	float y = (static_cast<float>(std::rand()) / RAND_MAX) * data.height;
	
	outputList.push_back({x, y});
	int countpoints = 0;
	std::vector<int> activeList;
	activeList.push_back(countpoints);

	float r = 25.0f, cellSize = r / sqrtf(2.0f);
	int cols = std::ceil(data.width / cellSize);
	int rows = std::ceil(data.height / cellSize);
	std::vector<int> PoissonGrid(cols * rows, -1);
	
	int gridX = static_cast<int>(x / cellSize);
	int gridY = static_cast<int>(y / cellSize);
	PoissonGrid[gridY * cols + gridX] = 0;

	int	index, point;
	float angle, len;

	while (activeList.size())
	{
		index = std::rand() % activeList.size();
		point = activeList[index];
		bool found = false;

		for (int i = 0; i < 30; ++i)
		{
			angle = 2.0f * f_PI * (static_cast<float>(std::rand()) / RAND_MAX);
			len = r + (static_cast<float>(std::rand()) / RAND_MAX) * r;
			x = len * cosf(angle) + outputList[point].x;
			y = len * sinf(angle) + outputList[point].y;

			if (x <= 0 || x >= data.width || y <= 0 || y >= data.height)
				continue;
			
			int cX = static_cast<int>(x / cellSize);
			int cY = static_cast<int>(y / cellSize);
			bool isValid = true;

			for (int j = -2; j <= 2; ++j)
			{
				for (int k = -2; k <= 2; ++k)
				{
					if (cX + j >= 0 && cX + j < cols && cY + k >= 0 && cY + k < rows)
					{
						int neighborID = PoissonGrid[(cY + k) * cols + cX + j];
						if (neighborID != -1)
						{
								float dx = x - outputList[neighborID].x;
								float dy = y - outputList[neighborID].y;
								if (dx * dx + dy * dy < r * r)
									isValid = false;
						}
					}
				}
			}
			if (isValid)
			{
				outputList.push_back({x, y});
				PoissonGrid[cY * cols + cX] = outputList.size() - 1;
				found = true;
				activeList.push_back(++countpoints);
				break;
			}
		}
		if (!found)
		{
			activeList[index] = activeList.back();
			activeList.pop_back();
		}
	}
}