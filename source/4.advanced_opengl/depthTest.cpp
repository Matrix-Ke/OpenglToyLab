#pragma once
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfwWrapper.h>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Defines.h"
#include "config.h"
#include "util/registerInput.h"
#include "util/opQueue.h"
#include "util/GStorage.H"
#include "util/lambdaOp.h"
#include "util/shader.h"
#include "util/commonUtil.h"
#include "util/filesystem.h"
#include "util/camera.h"
#include "util/model.h"

using namespace LOGL;
using namespace Oper;
using namespace Define;



// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	Glfw::GetInstance()->Init(SCR_WIDTH,SCR_HEIGHT, windowTitle.c_str());
	Glfw::GetInstance()->LockCursor();




	//build and compiler our shader program 
	//Shader ourShader("./shader/modelLoading/modelLoading.vs", "./shader/modelLoading/modelLoading.fs");


	// load models
	// -----------
	//Model ourModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));

	auto initOp = new LambdaOp([]() {
		glDepthFunc(GL_LESS);
	}, false);

	//------------ 输入
	auto registerInputOp = new RegisterInput(false);

	//------------- 时间
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	GStorage<float *>::GetInstance()->Register(str_DeltaTime.c_str(), &deltaTime);
	float lastFrame = 0.0f; // 上一帧的时间
	auto timeOp = new LambdaOp([&]() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	});

	//------------ 清除
	auto clearOp = new LambdaOp([]() {
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});


	//------------ 渲染
	auto renderOp = new OpQueue;
	(*renderOp) << clearOp;


	//------------- 末尾
	auto endOp = new LambdaOp([]() {
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});


	//------------- 整合
	auto opQueue = new OpQueue;
	(*opQueue) << initOp << registerInputOp << timeOp << renderOp << endOp;


	// render loop
	Glfw::GetInstance()->RenderLoop(opQueue);

	//------------
	Glfw::GetInstance()->Terminate();

	return 0;

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}


