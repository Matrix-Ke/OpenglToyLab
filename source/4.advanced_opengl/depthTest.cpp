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



	//注册相机
		//------------ 窗口
	float ratioWH = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 4.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);

	////设置几何物体
	VAO  cubeVAO(CubeVertices, sizeof(CubeVertices), { 3, 3, 2 });
	VAO  planeVAO(planeVertices, sizeof(planeVertices), { 3, 0, 2 });
	Shader cubeShader("./shader/advancedOpengl/depthTest.vs", "./shader/advancedOpengl/depthTest.fs");
	Shader cubeOutlineShader("./shader/advancedOpengl/stencilTest.vs", "./shader/advancedOpengl/stencilTest.fs");

	//Texture  texAwesome(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), true, false);
	//Texture  texMatrix(FileSystem::getPath("resources/textures/matrix.jpg").c_str(), true, false);

	Texture  texContainer(FileSystem::getPath("resources/textures/container2.png").c_str(), true, false);
	Texture  texContainerSpec(FileSystem::getPath("resources/textures/container2_specular.png").c_str(), true, false);
	Texture  planeTex(FileSystem::getPath("resources/textures/marble.jpg").c_str(), true, false);


	texContainer.SetTexNameInShader(cubeShader.getID(), "material.diffuse");
	planeTex.SetTexNameInShader(cubeShader.getID(), "material.diffuse");
	texContainerSpec.SetTexNameInShader(cubeShader.getID(), "material.specular");
	
	std::vector<Texture>  textures{ texContainer, texContainerSpec };
	Mesh  cubeMesh(cubeVAO, textures);


	std::vector<Texture>   pTexs{ planeTex };
	Mesh  planeMesh(planeVAO, pTexs);

	auto initOp = new LambdaOp([]() {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	}, false);

	initOp->Run();

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



	//设置物体材质， 环境灯光等
	auto  settingEnvir = new LambdaOp([&] {
		cubeShader.use();

		//绘制之前都必须设置渲染状态, 设置位置, 物体渲染(如果状态不复位可以放置在渲染循环之前)

		//全局属性面板
		cubeShader.setBool("blinn", true);

		//材质属性
		cubeShader.setFloat("material.shininess", material_shininess);

		// 灯光属性
		cubeShader.setVec3("light.position", lightPos);
		cubeShader.setVec3("light.ambient", light_ambient);
		cubeShader.setVec3("light.diffuse", light_diffuse);
		cubeShader.setVec3("light.specular", light_specular);

	});

	settingEnvir->Run();

	auto geomtryOp = new  LambdaOp([&]() {
		glStencilMask(0xFF); // 清除模板缓冲必须配合mask， 启用模板缓冲写入
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.7, 0.8, 0.9, 1.0);
		//glClearStencil(0);
		glEnable(GL_DEPTH_TEST);


		cubeShader.use();
		// world position  and  matrix transform
		glm::mat4 model = glm::mat4(1.0f);
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("projection", mainCamera.GetProjectionMatrix());
		cubeShader.setMat4("view", mainCamera.GetViewMatrix());
		cubeShader.setVec3("viewPos", mainCamera.GetPos());


		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0x00);
		planeMesh.Draw(cubeShader);

		////第一遍用来初始化 模板缓冲
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		cubeMesh.Draw(cubeShader);

		////第二遍开始缩放后开始绘制边框
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		float scale = 1.1f;
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		cubeOutlineShader.use();
		cubeOutlineShader.setMat4("model", model);
		cubeOutlineShader.setMat4("projection", mainCamera.GetProjectionMatrix());
		cubeOutlineShader.setMat4("view", mainCamera.GetViewMatrix());
		cubeOutlineShader.setVec3("viewPos", mainCamera.GetPos());
		cubeVAO.Use();
		cubeVAO.Draw();



		//恢复现场
		glStencilMask(0xFF); // 启用模板缓冲写入

	});


	//渲染队列
	auto renderQueue = new  OpQueue();
	*renderQueue << geomtryOp;


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


