//#pragma once
//#include <iostream>
//#include <string>
//
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <stb_image.h>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//
//#include "util/shader.h"
//#include "util/commonUtil.h"
//#include "util/filesystem.h"
//#include "util/camera.h"
//
//
//
//
//void framebufferSizeCallback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//
//void processInput(GLFWwindow* window);
//
//// settings
//const unsigned int SCR_WIDTH = 800;
//const unsigned int SCR_HEIGHT = 600;
//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f;	// time between current frame and last frame
//float lastFrame = 0.0f;
//
//
//int main()
//{
//	//init  environment and create window
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	GLFWwindow* window = glfwCreateWindow(800, 600, "openglToyLab", nullptr, nullptr);
//	if (window == nullptr)
//	{
//		std::cout << "create glfw  window failure" << std::endl;
//		glfwTerminate();
//
//		return  -1;
//	}
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
//	glfwSetCursorPosCallback(window, mouse_callback);
//	glfwSetCursorPosCallback(window, scroll_callback);
//
//	// tell GLFW to capture our mouse
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//
//	//anable depth test 
//	glEnable(GL_DEPTH_TEST);
//
//	//build and compiler our shader program 
//	Shader ourShader("./shader/getting_started/camera.vs", "./shader/getting_started/camera.fs");
//
//
//
//	float vertices[] = {
//			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//	};
//	// world space positions of our cubes
//	glm::vec3 cubePositions[] = {
//		glm::vec3(0.0f,  0.0f,  0.0f),
//		glm::vec3(2.0f,  5.0f, -15.0f),
//		glm::vec3(-1.5f, -2.2f, -2.5f),
//		glm::vec3(-3.8f, -2.0f, -12.3f),
//		glm::vec3(2.4f, -0.4f, -3.5f),
//		glm::vec3(-1.7f,  3.0f, -7.5f),
//		glm::vec3(1.3f, -2.0f, -2.5f),
//		glm::vec3(1.5f,  2.0f, -2.5f),
//		glm::vec3(1.5f,  0.2f, -1.5f),
//		glm::vec3(-1.3f,  1.0f, -1.5f)
//	};
//	unsigned int VBO, VAO;
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	// position attribute
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	// texture coord attribute
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//
//	// load and create a texture 
//	// -------------------------
//	unsigned int texture1, texture2;
//	// texture 1
//	// ---------
//	glGenTextures(1, &texture1);
//	glBindTexture(GL_TEXTURE_2D, texture1);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	int width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
//	unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
//	if (data)
//	{
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data);
//	// texture 2
//	// ---------
//	glGenTextures(1, &texture2);
//	glBindTexture(GL_TEXTURE_2D, texture2);
//	// set the texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	// load image, create texture and generate mipmaps
//	data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
//	if (data)
//	{
//		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else
//	{
//		std::cout << "Failed to load texture" << std::endl;
//	}
//	stbi_image_free(data);
//
//	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
//	// -------------------------------------------------------------------------------------------
//	ourShader.use();
//	ourShader.setInt("texture1", 0);
//	ourShader.setInt("texture2", 1);
//
//
//	while (!glfwWindowShouldClose(window))
//	{
//		//perframe time logic
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		// process input
//		processInput(window);
//
//		//render command  
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//		glClearColor(0.2, 0.4, 0.4, 1.0);
//
//		//bind texture unit
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture1);
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, texture2);
//
//		//draw call 
//		ourShader.use();
//
//		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT,0.1f, 100.0f );
//		ourShader.setMat4("projection", projection);
//		glm::mat4 view = camera.GetViewMatrix();
//		ourShader.setMat4("view", view);
//
//		glBindVertexArray(VAO);
//		for (unsigned int i = 0; i < 10 ; i++)
//		{
//			glm::mat4 model = glm::mat4(1.0f);
//			model = glm::translate(model, cubePositions[i]);
//			float angle = 20.0f *i;
//			ourShader.setMat4("model", model);
//
//			glDrawArrays(GL_TRIANGLES, 0, 36);
//
//		}
//
//		// poll event and swap buffer
//		glfwPollEvents();
//		glfwSwapBuffers(window);
//	}
//
//
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteBuffers(1, &VBO);
//
//
//
//	glfwTerminate();
//	return 0;
//}
//
//
//void framebufferSizeCallback(GLFWwindow* window, int width, int height)
//{
//	glViewport(200, 200, width, height);
//}
//
//void processInput(GLFWwindow* window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//		glfwSetWindowShouldClose(window, true);
//	}
//}
//
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos;
//
//	lastX = xpos;
//	lastY = ypos;
//
//	camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	camera.ProcessMouseScroll(yoffset);
//}