
#pragma once

#include "GLFW/glfwWrapper.h"

#include <iostream>
#include "util/GStorage.H"
#include "util/glDebug.h"

using namespace OpenGL;
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


	//对debug版本进行GLFW调试, 需要在调用glfwCreateWindow之前完成debug_output请求
#ifdef  _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	//------------
	GenWindow(width, height, title);
	LoadGL();


#ifdef _DEBUG
	// enable OpenGL debug context if context allows for debug context
	GLint flags; 
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		std::cout << "gl_context_flag_debug_bit is Enable" << std::endl;
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
		glDebugMessageCallback(glDebugOutput, nullptr);
		//opengl错误过滤， 在这里仅显示OpenGL API的高严重等级错误消息
		//glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
	}
#endif // DEBUG


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

	//-------------------
	glEnable(GL_DEPTH_TEST);
}


void Glfw::LockCursor() 
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Glfw::Terminate() { glfwTerminate(); }

GLFWwindow * Glfw::GetWindow() { return window; }


void Glfw::RenderLoop() 
{
	if (window == nullptr)
		return;
	//------------
	while (!glfwWindowShouldClose(window))
	{
		if (mForwardProcess && mForwardProcess->IsHold() )
		{
			mForwardProcess->Run();
		}
		if (mPostProcess && mPostProcess->IsHold())
		{
			mPostProcess->Run();
		}
	}
}



void Glfw::CB_FrameBuffSize(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}


void Glfw::GenWindow(size_t width, size_t height, const string & title)
{
	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (window == nullptr) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		Terminate();
		exit(1);
	}
	//------------
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, CB_FrameBuffSize);
}

void Glfw::LoadGL() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}
}

void OpenGL::Glfw::setRenderEnv(Oper::Ptr<Oper::Operation> op)
{
	
}

void OpenGL::Glfw::SetForwardProcess(Oper::Ptr<Oper::Operation> op)
{
	//Oper::Operation* ptr = this->mForwardProcess-><<
	if (mForwardProcess)
	{
		this->mForwardProcess->Push(op);
	}
	else
	{
		this->mForwardProcess = op;
	}
}

void OpenGL::Glfw::SetPostProcess(Oper::Ptr<Oper::Operation> op)
{
	//Oper::Operation* ptr = this->mForwardProcess-><<
	if (mPostProcess)
	{
		this->mPostProcess->Push(op);
	}
	else
	{
		this->mPostProcess = op;
	}
}

void OpenGL::Glfw::CloseWindow()
{
	glfwSetWindowShouldClose(window, true);
}

int OpenGL::Glfw::GetKey(int key)
{
	return glfwGetKey(window, key);
}
