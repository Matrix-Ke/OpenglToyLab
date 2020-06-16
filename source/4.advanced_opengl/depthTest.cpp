#pragma once
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfwWrapper.h>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "util/filesystem.h"
#include "Defines.h"
#include "config.h"
#include "util/registerInput.h"
#include "util/opQueue.h"
#include "util/GStorage.H"
#include "util/lambdaOp.h"
#include "util/shader.h"
#include "util/texture.h"
#include "util/commonUtil.h"
#include "util/VAO.h"
#include "util/model.h"
#include "util/camera.h"


using namespace LOGL;
using namespace Oper;
using namespace Define;
using namespace std;





int main()
{
	// glfw: initialize and configure
	Glfw::GetInstance()->Init(SCR_WIDTH,SCR_HEIGHT, windowTitle.c_str());
	//Glfw::GetInstance()->LockCursor();

	//-------------------
	glEnable(GL_DEPTH_TEST);


	//注册相机
		//------------ 窗口
	float ratioWH = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 4.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);

	////设置几何物体
	VAO  cubeVAO(CubeVertices, sizeof(CubeVertices), { 3, 3, 2 });
	VAO  planeVAO(planeVertices, sizeof(planeVertices), { 3, 0, 2 });
	Shader cubeShader("./shader/advancedOpengl/depthTest.vs", "./shader/advancedOpengl/depthTest.fs");

	Texture  texAwesome(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), true, false);
	Texture  texContainer(FileSystem::getPath("resources/textures/container2.png").c_str(), true, false);
	Texture  texContainerSpec(FileSystem::getPath("resources/textures/container2_specular.png").c_str(), true, false);
	Texture  texMatrix(FileSystem::getPath("resources/textures/matrix.jpg").c_str(), true, false);
	Texture  planeTex(FileSystem::getPath("resources/textures/marble.jpg").c_str(), true, false);

	

	auto initOp = new LambdaOp([]() {
		glDepthFunc(GL_LESS);
	}, false);

	//注册时间处理函数（需要和windows相应绑定处理各种输入）
	auto registerInputOp = new RegisterInput(false);
	registerInputOp->Run();


	//时间
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	GStorage<float *>::GetInstance()->Register(str_DeltaTime.c_str(), &deltaTime);
	float lastFrame = 0.0f; // 上一帧的时间
	auto timeOp = new LambdaOp([&]() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//std::cout << currentFrame << std::endl;
	});


	//清除颜色缓存
	auto  clearScreenOP = new LambdaOp([]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.7, 0.8, 0.9, 1.0);
	});


	//设置物体材质， 环境灯光等
	auto  settingEnvir = new LambdaOp([&] {
		cubeShader.use();

		//绘制之前都必须设置渲染状态, 设置位置, 物体渲染(如果状态不复位可以放置在渲染循环之前)
		cubeShader.setFloat("material.shininess", material_shininess);

		// lighting properties
		cubeShader.setVec3("light.position", lightPos);
		cubeShader.setVec3("light.ambient", light_ambient);
		cubeShader.setVec3("light.diffuse", light_diffuse);
		cubeShader.setVec3("light.specular", light_specular);

		cubeShader.setBool("blinn", true);
		cubeShader.setInt("material.diffuse", 0);
		cubeShader.setInt("material.specular", 1);


		texMatrix.Use(1);
	});

	settingEnvir->Run();

	auto geomtryOp = new  LambdaOp([&]() {
		cubeShader.use();


		glm::mat4 model = glm::mat4(1.0f);// world position
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("projection", mainCamera.GetProjectionMatrix());
		cubeShader.setMat4("view", mainCamera.GetViewMatrix());
		cubeShader.setVec3("viewPos", mainCamera.GetPos());

		
		texContainer.Use(0);
		cubeVAO.Use();
		cubeVAO.Draw();

		planeTex.Use(0);
		planeVAO.Use();
		planeVAO.Draw();
	});


	//渲染队列
	auto renderQueue = new  OpQueue();
	*renderQueue << clearScreenOP << geomtryOp;


	//swap buffer 
	auto endOp = new LambdaOp([]() {
		glfwPollEvents();
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
	});

	auto opLoop = new OpQueue();
	*opLoop << timeOp << renderQueue << endOp;

	Glfw::GetInstance()->RenderLoop(opLoop);

	Glfw::GetInstance()->Terminate();
	return 0;
}


