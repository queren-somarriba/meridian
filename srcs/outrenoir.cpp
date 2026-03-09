#include <ctime>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "noise.hpp"
#include "flowfield.hpp"
#include "engine.hpp"
#include "poissonDiskSampling.hpp"
#include "math_types.hpp"

bool fileExist(const std::string& name)
{
	std::ifstream f(name.c_str());
	return f.good();
}

bool loadConfig(const std::string& filename, otData& data)
{
	std::ifstream file(filename);
	
	if (!file.is_open())
	{
		std::cerr << "Error : Can't open configuration file" << filename << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line)) 
	{
		if (line.empty())
			continue;
		
		size_t commentPos = line.find("//");
		if (commentPos != std::string::npos)
			line = line.substr(0, commentPos);

		std::stringstream ss(line);
		std::string key, valueStr;

		if (std::getline(ss, key, ':') && std::getline(ss, valueStr)) 
		{
			key.erase(remove_if(key.begin(), key.end(), isspace), key.end());
			float value = std::stof(valueStr);

			if (key == "window_width")
				data.width = static_cast<int>(value);
			else if (key == "window_height")
				data.height = static_cast<int>(value);
			else if (key == "perlin_scale") 
				data.scale = value;
			else if (key == "step_size") 
				data.stepSize = value;
			else if (key == "line_padding") 
				data.line_padding = value;
			else if (key == "pds_radius") 
				data.pds_radius = value;
			else if (key == "speed") 
				data.speed = value;
			else if (key == "record") 
				data.record = value;
		}
	}

	if (!data.width || !data.height || !data.scale || !data.stepSize
		|| !data.line_padding || !data.pds_radius || !data.speed)
		return false;
	return true;
}

int main(int argc, char** argv)
{
	renderContext	context;
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
	{
		int fbWidth, fbHeight;
		std::string	name = "outrenoir_";
		std::string	extName = ".mp4";
		int nb = 0;
		std::string	fileName = name + std::to_string(nb) + extName;
		
		while (fileExist(fileName))
		{
			++nb;
			fileName = name + std::to_string(nb) + extName;
		}	
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		std::string cmd = "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s " + std::to_string(fbWidth) + "x" + std::to_string(fbHeight) + " -r 60 -i - -vf vflip -c:v libx264 -pix_fmt yuv420p " + fileName;
		context.ffmpegPipe = popen(cmd.c_str(), "w");
		context.isRecording = true;
	

		if (!context.ffmpegPipe)
			std::cerr << "Erreur : Impossible d'ouvrir le pipe FFmpeg. Est-il installé ?" << std::endl;
	}

	context.u_angle = (static_cast<float>(std::rand()) / RAND_MAX * 10.0f);

	while (!glfwWindowShouldClose(window))
		renderScene(window, context, data);

	glDeleteBuffers(1, &context.VBO);
	glfwTerminate();

	return 0;
}