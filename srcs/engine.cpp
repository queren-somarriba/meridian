#include "engine.hpp"

GLFWwindow* initWindow(const otData& data)
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

	GLFWwindow* window = glfwCreateWindow(data.width, data.height, "outrenoir", NULL, NULL);
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

	glClearColor(28.0 / 255.0, 28.0 / 255.0, 28.0 / 255.0, 1.0);

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
		"float noise(vec2 st) {\n"
		"vec2 i = floor(st);\n"
		"vec2 f = fract(st);\n"
		"float a = random(i);\n"
		"float b = random(i + vec2(1.0, 0.0));\n"
		"float c = random(i + vec2(0.0, 1.0));\n"
		"float d = random(i + vec2(1.0, 1.0));\n"
		"vec2 u = f * f * (3.0 - 2.0 * f);\n"
		"return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;\n"
		"}\n"
		"float fbm(vec2 st) {\n"
		"float value = 0.0;\n"
		"float amplitude = 0.5;\n"
		"for (int i = 0; i < 5; i++) {\n"
		"	value += amplitude * noise(st);\n"
		"	st *= 2.0;\n"
		"	amplitude *= 0.5;\n"
		"}\n"
		"return value;\n"
		"}\n"
		"uniform float u_angle;\n"
		"in vec2 v_uv;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"vec3 colorA = vec3(254.0 / 255.0, 254.0 / 255.0, 244.0 / 255.0);\n"
    		"vec3 colorB = vec3(110.0 / 255.0, 110.0 / 255.0, 110.0 / 255.0);\n"
		"float mixFactor = sin((v_uv.x + v_uv.y) * 3.0 + u_angle) * 0.5 + 0.5;\n"
		"vec3 finalColor = mix(colorA, colorB, mixFactor);\n"
		"float rawFbm = fbm(v_uv * 500.0);\n"
		"float paperGrain = mix(0.85, 1.15, rawFbm);\n"
		"FragColor = vec4(finalColor * paperGrain, 1.0);\n"
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

void renderScene(GLFWwindow* window, renderContext& context, const otData& data)
{
	int currentWidth, currentHeight;
	glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
	glViewport(0, 0, currentWidth, currentHeight);

	if (context.currentDrawCount < context.vertexCount)
	{
		context.currentDrawCount += context.DrawSpeed;
		if (context.currentDrawCount > context.vertexCount)
		{
			context.currentDrawCount = context.vertexCount;
			if (context.isRecording && context.currentDrawCount >= context.vertexCount)
			{
				std::cout << "Animation terminée, finalisation de la vidéo..." << std::endl;
				pclose(context.ffmpegPipe);
				context.ffmpegPipe = nullptr;
				context.isRecording = false;
				std::cout << "Vidéo sauvegardée sous outrenoir.mp4 !" << std::endl;
			}
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(context.shaderProgram);

	GLint u_resolutionLocaltion = glGetUniformLocation(context.shaderProgram, "u_resolution");
	glUniform2f(u_resolutionLocaltion, static_cast<float>(data.width), static_cast<float>(data.height));

	GLint u_angleLocation = glGetUniformLocation(context.shaderProgram, "u_angle");
	glUniform1f(u_angleLocation, context.u_angle);

	glBindVertexArray(context.VAO);

	glDrawArrays(GL_TRIANGLES, 0, context.currentDrawCount);

	if (context.isRecording && context.ffmpegPipe)
	{
		std::vector<unsigned char> pixels(currentWidth * currentHeight * 3);
		glReadPixels(0, 0, currentWidth, currentHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
		fwrite(pixels.data(), 1, pixels.size(), context.ffmpegPipe);
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}
