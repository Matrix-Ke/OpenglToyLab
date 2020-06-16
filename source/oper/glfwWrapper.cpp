#pragma once

#include "GLFW/glfwWrapper.h"

#include <iostream>
#include "util/GStorage.H"

using namespace LOGL;
using namespace Oper;
using namespace std;

Glfw::Glfw() : window(nullptr) {};

//------------

Glfw* Glfw::m_Instance = new Glfw();

Glfw* Glfw::GetInstance() { return m_Instance; }


void  Glfw::Init(size_t width /* = 800 */, size_t height /* = 600 */, const std::string & title /* = "title" */)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//------------
	GenWindow(width, height, title);
	LoadGL();
	glViewport(0, 0, width, height);


	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scanCode, int state, int mods) {
		size_t kbState =
			(state == GLFW_PRESS ? EventManager::KEYBOARD_PRESS
				: (state == GLFW_REPEAT ? EventManager::KEYBOARD_REPEAT
					: (state == GLFW_RELEASE ? EventManager::KEYBOARD_RELEASE
						: 0)));
		if (kbState != 0)
			EventManager::GetInstance()->Response(key | kbState);
		std::cout << "Oper::EventManager::KEYBOARD has been called " << std::endl;
		EventManager::GetInstance()->Response(key | Oper::EventManager::KEYBOARD);
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
		static float lastX = xPos, lastY = yPos;
		static float mousePos_XOffset, mousePos_YOffset;
		GStorage<float *>::GetInstance()->Register("mousePos_XOffset", &mousePos_XOffset);
		GStorage<float *>::GetInstance()->Register("mousePos_YOffset", &mousePos_YOffset);
		//------------
		mousePos_XOffset = xPos - lastX;
		mousePos_YOffset = lastY - yPos;
		lastX = xPos;
		lastY = yPos;
		EventManager::GetInstance()->Response(EventManager::MOUSE_MOUVE);
	});

	//------------
	glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
		static float mouseScroll_YOffset;
		GStorage<float *>::GetInstance()->Register("mouseScroll_YOffset", &mouseScroll_YOffset);
		//------------
		mouseScroll_YOffset = yOffset;
		EventManager::GetInstance()->Response(EventManager::MOUSE_SCROLL);
	});
}


void Glfw::LockCursor() {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Glfw::Terminate() { glfwTerminate(); }

GLFWwindow * Glfw::GetWindow() { return window; }


void Glfw::RenderLoop(Ptr<Operation>  op) {
	if (window == nullptr)
		Init();
	//------------
	bool loop = op != nullptr;
	while (!glfwWindowShouldClose(window)) {
		if (loop) {
			op->Run();
			if (!op->IsHold())
				loop = false;
		}
	}
}


void Glfw::RenderLoop(Operation * op) {
	RenderLoop(Operation::ToPtr(op));
}

void Glfw::CB_FrameBuffSize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void Glfw::GenWindow(size_t width, size_t height, const string & title) {
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		Terminate();
		exit(1);
	}
	//------------
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, CB_FrameBuffSize);
}

void Glfw::LoadGL() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}
}

void LOGL::Glfw::CloseWindow()
{
	glfwSetWindowShouldClose(window, true);
}

int LOGL::Glfw::GetKey(int key)
{
	return glfwGetKey(window, key);
}
