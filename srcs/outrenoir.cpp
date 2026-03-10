#include <ctime>
#include <cstdio>
#include <iostream>

#include "flowfield.hpp"
#include "engine.hpp"
#include "math_types.hpp"
#include "ffmpegRecorder.hpp"
#include "fileUtils.hpp"

bool setupData(int argc, char** argv, otData& data)
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./outrenoir conf/<file>" << std::endl;
		return false;
	}

	std::string configFile = (argc > 1) ? argv[1] : "config.txt";

	if (!loadConfig(configFile, data))
		return false;

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	data.offsetX = static_cast<float>(std::rand() % 10000);
	data.offsetY = static_cast<float>(std::rand() % 10000);
	
	return true;
}

bool initGraphics(renderContext& context, std::vector<vec2>& allSegments, const otData& data)
{
	context.vertexCount = allSegments.size();
	SetupBuffers(allSegments, context.VAO, context.VBO);
	context.shaderProgram = CompileShaders();
	if (!context.shaderProgram)
        {
                glDeleteBuffers(1, &context.VBO);
                glfwTerminate();
                return false; 
        }

	context.DrawSpeed = 600.0f * data.speed;
	context.currentDrawCount = 0;

	return true;
}

int main(int argc, char** argv)
{
	otData	data = {};

	if (!setupData(argc, argv, data))
		return 1;

	GLFWwindow* window = initWindow(data);
	if (!window)
		return -1;

	std::vector<vec2> allSegments = flowfield(data);

	renderContext	context = {};
	if (!initGraphics(context, allSegments, data))
	{
		glfwTerminate();
		return 1; 
	}

	if (data.record)
		startffmpeg(window, context);

	context.u_angle = (static_cast<float>(std::rand()) / RAND_MAX * 10.0f);

	while (!glfwWindowShouldClose(window))
		renderScene(window, context, data);

	glDeleteBuffers(1, &context.VBO);
	glfwTerminate();

	return 0;
}