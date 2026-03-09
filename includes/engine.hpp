#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "flowfield.hpp"
#include "math_types.hpp"

struct renderContext
{
	FILE*	ffmpegPipe;
	float	u_angle;
	GLuint	VAO;
	GLuint	VBO;
	GLuint	shaderProgram;
	int	vertexCount;
	int	currentDrawCount;
	int	DrawSpeed;
	bool	isRecording;
};

GLFWwindow*	initWindow(const otData& data);

void		SetupBuffers(const std::vector<vec2>& line, GLuint& VAO, GLuint& VBO);

GLuint		CompileShaders();

void		renderScene(GLFWwindow* window , renderContext& context, const otData& data);