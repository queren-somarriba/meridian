#version 330 core

layout (location = 0) in vec2 aPos;
uniform vec2 u_resolution;
out vec2 v_uv;

void main()
{
	vec2 normalizedPos = (aPos / u_resolution) * 2.0 - 1.0;
	normalizedPos.y *= -1.0;
	gl_Position = vec4(normalizedPos, 0.0, 1.0);
	v_uv = aPos / u_resolution;
}