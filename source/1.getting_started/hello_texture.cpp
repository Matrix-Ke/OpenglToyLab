//#pragma once
//#include <iostream>
//#include <string>
//
//#include <glad/glad.h>
//#include <GLFW/glfwWrapper.h>
//#include <stb_image.h>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "util/filesystem.h"
//#include "Defines.h"
//#include "config.h"
//#include "util/registerInput.h"
//#include "util/opQueue.h"
//#include "util/GStorage.H"
//#include "util/lambdaOp.h"
//#include "util/shader.h"
//#include "util/texture.h"
//#include "util/commonUtil.h"
//#include "util/VAO.h"
//#include "util/model.h"
//#include "util/camera.h"
//
//
//using namespace LOGL;
//using namespace Oper;
//using namespace Define;
//
//
//
//
//
//int main()
//{
//	// glfw: initialize and configure
//	Glfw::GetInstance()->Init(SCR_WIDTH, SCR_HEIGHT, windowTitle.c_str());
//	//Glfw::GetInstance()->LockCursor();
//
//
//	//注册相机
//	float ratioWH = (float)SCR_WIDTH / (float)SCR_HEIGHT;
//	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 4.0f));
//	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);
//
//
//	VAO		cubeVAO(quadVertices, sizeof(quadVertices), { 3, 3, 2 }, quadIndices, sizeof(quadIndices));
//	Shader	cubeShader("./shader/getting_started/textureShader.vs", "./shader/getting_started/textureShader.fs");
//	Texture  tex2(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), true, false);
//	Texture  tex1(FileSystem::getPath("resources/textures/container.jpg").c_str(), true, false);
//
//	auto initOp = new LambdaOp([]() {
//		glDepthFunc(GL_LESS);
//	}, false);
//
//
//	//注册时间处理函数（需要和windows相应绑定处理各种输入）
//	auto registerInputOp = new RegisterInput(false);
//	registerInputOp->Run();
//
//
//	//时间
//	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
//	GStorage<float *>::GetInstance()->Register(str_DeltaTime.c_str(), &deltaTime);
//	float lastFrame = 0.0f; // 上一帧的时间
//	auto timeOp = new LambdaOp([&]() {
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//	});
//
//
//
//	//清除颜色缓存
//	auto  clearScreenOP = new LambdaOp([]() {
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glClearColor(0.2, 0.4, 0.4, 1.0);
//	});
//
//
//	auto geomtryOp = new  LambdaOp([&]() {
//		cubeShader.use();
//		cubeVAO.Use();
//		//每次绘制之前都必须设置渲染状态, 设置位置
//		cubeShader.setInt("texture1", 0);
//		cubeShader.setInt("texture2", 1);
//		tex1.Use(0);
//		tex2.Use(1);
//
//		cubeVAO.Draw();
//
//	});
//
//	//渲染队列
//	auto renderQueue = new  OpQueue();
//	*renderQueue << clearScreenOP << geomtryOp;
//
//	//swap buffer 
//	auto endOp = new LambdaOp([]() {
//		glfwPollEvents();
//		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
//	});
//
//	auto opLoop = new OpQueue();
//	*opLoop << timeOp << renderQueue << endOp;
//
//
//	// 渲染循环
//	Glfw::GetInstance()->RenderLoop(opLoop);
//
//
//	Glfw::GetInstance()->Terminate();
//	return 0;
//}
//
//
