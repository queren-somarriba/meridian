#include <ctime>
#include <cstdio>
#include <iostream>
#include <string>

#include "noise.hpp"
#include "flowfield.hpp"
#include "engine.hpp"
#include "poissonDiskSampling.hpp"
#include "math_types.hpp"
#include "ffmpegRecorder.hpp"
#include "fileUtils.hpp"


int main(int argc, char** argv)
{
	renderContext	context = {};
	otData	data = {};

	if (argc != 2)
	{
		std::cerr << "Usage: ./outrenoir conf/<file>" << std::endl;
		return 1;
	}

	std::string configFile = (argc > 1) ? argv[1] : "config.txt";

	if (!loadConfig(configFile, data))
		return 1;

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	data.offsetX = static_cast<float>(std::rand() % 10000);
	data.offsetY = static_cast<float>(std::rand() % 10000);

	GLFWwindow* window = initWindow(data);
	if (!window)
		return -1;

	std::vector<vec2> grid(data.width * data.height);
	grid = makeGrid(data);

	std::vector<vec2> outputList;
	poissonDiskSampling(outputList, data);

	std::vector<vec2> allSegments;
	allSegments.reserve(2000 * 500 * 6);

	collisionContext col_ctx;
	col_ctx.cellSize = data.line_padding / sqrtf(2.0f);
	col_ctx.cols = std::ceil(data.width / col_ctx.cellSize);
	col_ctx.rows = std::ceil(data.height / col_ctx.cellSize);
	col_ctx.grid = std::vector<int>(col_ctx.cols * col_ctx.rows, -1);


	for (int i = 0; i < static_cast<int>(outputList.size()); ++i)
		makeSegments({outputList[i].x, outputList[i].y}, data, grid, allSegments, i, col_ctx);

	context.vertexCount = allSegments.size();
	SetupBuffers(allSegments, context.VAO, context.VBO);
	context.shaderProgram = CompileShaders();

	context.DrawSpeed = 600.0f * data.speed;
	context.currentDrawCount = 0;

	if (data.record)
		startffmpeg(window, context);

	context.u_angle = (static_cast<float>(std::rand()) / RAND_MAX * 10.0f);

	while (!glfwWindowShouldClose(window))
		renderScene(window, context, data);

	glDeleteBuffers(1, &context.VBO);
	glfwTerminate();

	return 0;
}