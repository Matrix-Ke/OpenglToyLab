#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "util/shader.h"
#include "util/commonUtil.h"



void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main()
{
	//init  environment and create windows
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* windows = glfwCreateWindow(800, 600, "openglToyLab", nullptr, nullptr);
	if (windows == nullptr)
	{
		std::cout << "create glfw  windows failure" << std::endl;
		glfwTerminate();

		return  -1;
	}
	glfwMakeContextCurrent(windows);
	glfwSetFramebufferSizeCallback(windows, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//build and compiler our shader program 
	//std::string  rootdir = std::string(getCurrentDir());
	//std::string  vShaderPath = rootdir + "/shader/shader.vs";
	//std::string  fShaderPath = rootdir + "/shader/shader.fs";
	//Shader ourShader(vShaderPath.c_str(), fShaderPath.c_str());
	Shader ourShader("./shader/shader.vs", "./shader/shader.fs");



	//set vertex date and  set vbo
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);









	while (!glfwWindowShouldClose(windows))
	{
		// process input
		processInput(windows);

		//render command  
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2, 0.4, 0.4, 1.0);

		//draw call 
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// poll event and swap buffer
		glfwPollEvents();
		glfwSwapBuffers(windows);
	}

	glfwTerminate();
	return 0;
}


void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(200, 200, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}