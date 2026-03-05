#include "engine.hpp"

GLFWwindow* initWindow()
{
	if (!glfwInit())
	{
		std::cerr << "Échec de l'initialisation de GLFW" << std::endl;
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Meridian", NULL, NULL);
	if (!window)
	{
		std::cerr << "Échec de la création de la fenêtre GLFW" << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		std::cerr << "Erreur: " << glewGetErrorString(res) << std::endl;
		return NULL;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(254.0, 254.0, 226.0, 1.0);

	return window;
}

void SetupBuffers(const std::vector<vec2>& allSegments, GLuint& VAO, GLuint& VBO)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, allSegments.size() * sizeof(vec2), allSegments.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLuint CompileShaders()
{
	
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec2 aPos;\n"
		"uniform vec2 u_resolution;\n"
		"out vec2 v_uv;\n"
		"void main()\n"
		"{\n"
		"   vec2 normalizedPos = (aPos / u_resolution) * 2.0 - 1.0;\n"
		"   normalizedPos.y *= -1.0;\n"
		"   gl_Position = vec4(normalizedPos, 0.0, 1.0);\n"
		"v_uv = aPos / u_resolution;\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"float random(vec2 p) {\n"
		"vec3 p3  = fract(vec3(p.xyx) * 0.1031);\n"
		"p3 += dot(p3, p3.yzx + 33.33);\n"
		"return fract((p3.x + p3.y) * p3.z);}\n"
		"in vec2 v_uv;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"vec3 colorA = vec3(127.0 / 255.0, 143.0 / 255.0, 166.0 / 255.0);\n"
    		"vec3 colorB = vec3(48.0 / 255.0, 48.0 / 255.0, 48.0 / 255.0);\n"
		"float noiseValue = random(gl_FragCoord.xy) * 0.15;\n"
		"vec3 finalColor = mix(colorA, colorB, v_uv.x);\n"
		"   FragColor = vec4(finalColor, 0.8) - noiseValue;\n"
		"}\0";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram); 

	return shaderProgram;
}

void renderScene(GLFWwindow* window, renderContext& context)
{
	int currentWidth, currentHeight;
	glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
	glViewport(0, 0, currentWidth, currentHeight);

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(context.shaderProgram);

	GLint u_resolutionLocaltion = glGetUniformLocation(context.shaderProgram, "u_resolution");
	glUniform2f(u_resolutionLocaltion, static_cast<float>(WIDTH), static_cast<float>(HEIGHT));

	glBindVertexArray(context.VAO);
	glDrawArrays(GL_TRIANGLES, 0, context.vertexCount);

	glfwSwapBuffers(window);
	glfwPollEvents();
}
