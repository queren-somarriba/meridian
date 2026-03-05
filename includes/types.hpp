#pragma once
#include <GL/glew.h>

#define f_PI 3.14159265f

struct vec2
{
	float	x;
	float	y;
};

// struct triangles
// {
// 	vec2 	s1;
// 	vec2	s2;
// 	vec2	s3;
// };

struct meridiansData
{
	int	width;
	int	height;
	float	scale;
	float	stepSize;
};

struct renderContext
{
	GLuint	VAO;
	GLuint	VBO;
	GLuint	shaderProgram;
	int	vertexCount;
};