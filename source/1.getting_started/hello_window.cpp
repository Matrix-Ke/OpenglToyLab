#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


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

	while (!glfwWindowShouldClose(windows))
	{
		// process input
		processInput(windows);

		//render command  
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2, 0.4, 0.4, 1.0);



		// poll event and swap buffer
		glfwPollEvents();
		glfwSwapBuffers(windows);
	}

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