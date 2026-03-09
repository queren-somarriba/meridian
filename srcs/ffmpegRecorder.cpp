#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "fileUtils.hpp"
#include "engine.hpp"

void startffmpeg(GLFWwindow* window, renderContext& context)
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

	std::string cmd = "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s " + std::to_string(fbWidth);
	cmd += "x" + std::to_string(fbHeight) + " -r 60 -i - -vf vflip -c:v libx264 -pix_fmt yuv420p " + fileName;
	
	context.ffmpegPipe = popen(cmd.c_str(), "w");
	context.isRecording = true;

	if (!context.ffmpegPipe)
		std::cerr << "Error : can't open FFmpeg pipe." << std::endl;
}

void record(renderContext& context, int currentWidth, int currentHeight)
{
	std::vector<unsigned char> pixels(currentWidth * currentHeight * 3);
	glReadPixels(0, 0, currentWidth, currentHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
	fwrite(pixels.data(), 1, pixels.size(), context.ffmpegPipe);
}

void closeRecorder(renderContext& context)
{
	std::cout << "Animation finished, finalization of the video..." << std::endl;
	pclose(context.ffmpegPipe);
	context.ffmpegPipe = nullptr;
	context.isRecording = false;
	std::cout << "Video saved as outrenoir.mp4 !" << std::endl;
}