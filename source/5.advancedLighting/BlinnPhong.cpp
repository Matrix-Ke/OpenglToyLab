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
	Glfw::GetInstance()->LockCursor();


	//注册相机, 窗口
	float ratioWH = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 4.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);

	////设置几何物体
	VAO  planeVAO(planeVertices, sizeof(planeVertices), { 3, 3, 2 });


	Shader BlinnPhongShader("./shader/advancedLighting/BlinnPhong.vs", "./shader/advancedLighting/BlinnPhong.fs");
	GStorage<Shader*>::GetInstance()->Register(str_BlinnPhong, &BlinnPhongShader);
	BlinnPhongShader.BindUniformBlockIndex("Matrices", 0);


	Texture  planeTex(FileSystem::getPath("resources/textures/wood.png").c_str(), true, false);
	planeTex.SetName("texture_diffuse1");

	//网格物体
	Mesh	planeMesh(planeVAO, planeTex);


	//创建uniform buffer object 
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//绑定到绑定点
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);



	//注册处理函数（需要和windows相应绑定处理各种输入）
	auto registerInputOp = new RegisterInput(false);
	registerInputOp->Run();

	
	//渲染环境配置
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

	//设置物体材质， 环境灯光等
	auto  settingEnvir = new LambdaOp([&] {
		BlinnPhongShader.use();
		//绘制之前都必须设置渲染状态, 设置位置, 物体渲染(如果状态不复位可以放置在渲染循环之前)
		//全局属性面板
		BlinnPhongShader.setBool("blinn", true);

		//材质属性
		BlinnPhongShader.setFloat("material.shininess", material_shininess);

		// 灯光属性
		BlinnPhongShader.setVec3("light.position", lightPos);
		BlinnPhongShader.setVec3("light.ambient", light_ambient);
		BlinnPhongShader.setVec3("light.diffuse", light_diffuse);
		BlinnPhongShader.setVec3("light.specular", light_specular);

	});
	settingEnvir->Run();


	//更新存储的时间差。
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	GStorage<float *>::GetInstance()->Register(str_DeltaTime.c_str(), &deltaTime);
	float lastFrame = 0.0f; // 上一帧的时间
	auto timeOp = new LambdaOp([&]() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//std::cout << currentFrame << std::endl;
	});


	glm::mat4   uboMat4Arrays[2];
	auto geomtryOp = new  LambdaOp([&]() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//更新ubo, 可以采取批量更新也可以逐次更新uniform 
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		uboMat4Arrays[0] = mainCamera.GetProjectionMatrix();
		uboMat4Arrays[1] = mainCamera.GetViewMatrix();
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::mat4), uboMat4Arrays);
		//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mainCamera.GetProjectionMatrix()));
		//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mainCamera.GetViewMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		BlinnPhongShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		BlinnPhongShader.setMat4("model", model);
		BlinnPhongShader.setVec3("viewPos", mainCamera.GetPos());
		planeMesh.Draw(BlinnPhongShader);

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


