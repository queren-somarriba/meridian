#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "types.hpp"

GLFWwindow*	initWindow();

void		SetupBuffers(const std::vector<vec2>& line, GLuint& VAO, GLuint& VBO);

GLuint		CompileShaders();

void		renderScene(GLFWwindow* window , renderContext& context);