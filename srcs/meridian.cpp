#include "meridian.hpp"

int main(int argc, char** argv)
{
	renderContext	context;
	meridianData	data;

	if (argc < 3)
	{
		std::cerr << "Usage: ./meridian <scale> <setpSize>" << std::endl;//stepSize, nombre de lignes, seed
		return 1;
	}

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	data.scale = std::stof(argv[1]);//scale between 0.0f and 1.0f
	data.stepSize = std::stof(argv[2]);

	GLFWwindow* window = initWindow();
	if (!window)
		return -1;

	std::vector<vec2> grid(WIDTH * HEIGHT);
	grid = makeGrid(data);

	std::vector<vec2> outputList;
	poissonDiskSampling(outputList);

	std::vector<vec2> allSegments;
	allSegments.reserve(2000 * 500 * 6);

	collisionContext col_ctx;
	col_ctx.cellSize = LINE_PADDING / sqrtf(2.0f);
	col_ctx.cols = std::ceil(WIDTH / col_ctx.cellSize);
	col_ctx.rows = std::ceil(HEIGHT / col_ctx.cellSize);
	col_ctx.grid = std::vector<int>(col_ctx.cols * col_ctx.rows, -1);


	for (int i = 0; i < static_cast<int>(outputList.size()); ++i)
		makeSegments({outputList[i].x, outputList[i].y}, data, grid, allSegments, i, col_ctx);

	context.vertexCount = allSegments.size();
	SetupBuffers(allSegments, context.VAO, context.VBO);
	context.shaderProgram = CompileShaders();

	while (!glfwWindowShouldClose(window))
		renderScene(window, context);

	glDeleteBuffers(1, &context.VBO);
	glfwTerminate();

	return 0;
}