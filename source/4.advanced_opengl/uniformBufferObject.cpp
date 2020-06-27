#pragma once
#include <iostream>
#include <string>
#include <functional>

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
#include "util/fbo.h"
#include "util/model.h"


using namespace OpenGL;
using namespace Oper;
using namespace Define;
using namespace std;


//开始绘制透明物体, 需要对物体到相机的距离做一个排序		//强制改变排序方式
std::function<bool(const float &, const float &)>   com_greater([](const float & f1, const float &f2)->bool {
	if (f1 < f2)
		return false;
	else
		return true;
});


int main()
{
	// glfw: initialize and configure
	Glfw::GetInstance()->Init(SCR_WIDTH, SCR_HEIGHT, windowTitle.c_str());
	//Glfw::GetInstance()->LockCursor();



	//注册相机
		//------------ 窗口
	float ratioWH = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 4.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);

	////设置几何物体
	VAO  cubeVAO(CubeVertices, sizeof(CubeVertices), { 3, 3, 2 });
	VAO  planeVAO(planeVertices, sizeof(planeVertices), { 3, 0, 2 });
	VAO  skyboxVAO(skyboxVertices, sizeof(skyboxVertices), { 3 });
	VAO  quadTransparentVAO(transparentVertices, sizeof(transparentVertices), { 3, 0, 2 });

	Shader cubeShader("./shader/advancedOpengl/depthTest.vs", "./shader/advancedOpengl/depthTest.fs");
	Shader skyBoxShader("./shader/advancedOpengl/skyBox.vs", "./shader/advancedOpengl/skyBox.fs");
	Shader blendlingTestShader("./shader/advancedOpengl/blending_discard.vs", "./shader/advancedOpengl/blending_discard.fs");
	cubeShader.BindUniformBlockIndex("Matrices", 0);
	blendlingTestShader.BindUniformBlockIndex("Matrices", 0);

	Texture  texContainer(FileSystem::getPath("resources/textures/container2.png").c_str(), true, false);
	Texture  texContainerSpec(FileSystem::getPath("resources/textures/container2_specular.png").c_str(), true, false);
	Texture  planeTex(FileSystem::getPath("resources/textures/marble.jpg").c_str(), true, false);
	Texture  texWindows(FileSystem::getPath("resources/textures/window.png").c_str(), false, false);

	vector<std::string> faces
	{
		FileSystem::getPath("resources/textures/skybox/right.jpg"),
		FileSystem::getPath("resources/textures/skybox/left.jpg"),
		FileSystem::getPath("resources/textures/skybox/top.jpg"),
		FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
		FileSystem::getPath("resources/textures/skybox/front.jpg"),
		FileSystem::getPath("resources/textures/skybox/back.jpg")
	};
	Texture		skyBoxTexture(faces);


	texContainer.SetName("material.diffuse");
	texContainerSpec.SetName("material.specular");
	planeTex.SetName("material.diffuse");
	skyBoxTexture.SetName("skybox");
	texWindows.SetName("texture1");


	std::vector<Texture>  textures{ texContainer, texContainerSpec };
	Mesh	cubeMesh(cubeVAO, textures);
	Mesh	planeMesh(planeVAO, planeTex);
	Mesh	meshSkybox(skyboxVAO, skyBoxTexture);
	Mesh	quadMesh(quadTransparentVAO, texWindows);

	//创建uniform buffer object 
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//绑定到绑定点
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);




	auto initOp = new LambdaOp([]() {
		//开启深度测试
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		//开启混合模式
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		//开启背面剔除
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW); // gl_ccw 代表的是逆时针的环绕方式

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.7, 0.8, 0.9, 1.0);

		//更新ubo
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mainCamera.GetProjectionMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mainCamera.GetViewMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		cubeShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		cubeShader.setMat4("model", model);
		cubeShader.setVec3("viewPos", mainCamera.GetPos());
		planeMesh.Draw(cubeShader);
		cubeMesh.Draw(cubeShader);


		skyBoxShader.use();
		//绘制天空盒的时候，需要移除移动矩阵,还需要对shader设置z坐标位1.0；
		//修改等于时, 深度测试也通过.early-Z test
		glDepthFunc(GL_LEQUAL);
		glm::mat4  view = glm::mat4(glm::mat3(mainCamera.GetViewMatrix()));
		skyBoxShader.setMat4("view", view);
		skyBoxShader.setMat4("projection", mainCamera.GetProjectionMatrix());
		skyBoxShader.setVec3("viewPos", mainCamera.GetPos());
		meshSkybox.Draw(skyBoxShader);
		//恢复默认值
		glDepthFunc(GL_LESS);


		//绘制半透明物体
		std::map<float, glm::vec3, std::function<bool(const float &, const float &)> > stored(com_greater);
		for (auto & vecIter : Translation)
		{
			float distanceC2G = glm::distance(mainCamera.GetPos(), vecIter);
			stored.insert(std::pair<float, glm::vec3>(distanceC2G, vecIter));
		}
		blendlingTestShader.use();
		blendlingTestShader.setVec3("viewPos", mainCamera.GetPos());
		glm::mat4  tmpModel(1.0f);
		for (auto vecIter : stored)
		{
			tmpModel = glm::mat4(1.0f);
			glm::vec3  tmp(vecIter.second);
			tmpModel = glm::translate(tmpModel, tmp);
			blendlingTestShader.setMat4("model", tmpModel);
			quadMesh.Draw(blendlingTestShader);
		}

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


