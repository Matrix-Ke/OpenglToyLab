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
#include "util/sphere.h"


using namespace OpenGL;
using namespace Oper;
using namespace Define;
using namespace std;
using namespace BasicShape;


int main()
{

	// glfw: initialize and configure
	Glfw::GetInstance()->Init(SCR_WIDTH, SCR_HEIGHT, windowTitle.c_str());
	//Glfw::GetInstance()->LockCursor();

		//注册相机, 窗口
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 3.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);


	Shader equirectangularToCubemapShader("./shader/IBL/cubemap.vs", "./shader/IBL/equirectangular_to_cubemap.fs");
	Shader backgroundShader("./shader/IBL/background.vs", "./shader/IBL/background.fs");
	Shader pbrShader("./shader/IBL/pbr.vs", "./shader/IBL/pbr.fs");

	//设置几何物体, 创建球体网格
	Sphere   baseSphere(30);
	std::vector<glm::vec3>  position = baseSphere.GetVertexArr();
	std::vector<glm::vec3>  normal = baseSphere.GetNormalArr();
	std::vector<glm::vec2>  texcoord = baseSphere.GetTexCoordsArr();
	std::vector<glm::uvec3> indexArray = baseSphere.GetIndexArr();
	std::vector<float>  sphereVertices;
	for (unsigned int index = 0; index < position.size(); ++index)
	{
		sphereVertices.push_back(position[index].x);
		sphereVertices.push_back(position[index].y);
		sphereVertices.push_back(position[index].z);

		sphereVertices.push_back(normal[index].x);
		sphereVertices.push_back(normal[index].y);
		sphereVertices.push_back(normal[index].z);

		sphereVertices.push_back(texcoord[index].x);
		sphereVertices.push_back(texcoord[index].y);
	};
	VAO  sphereVAO(&sphereVertices[0], sizeof(float) * sphereVertices.size(), { 3, 3, 2 }, &indexArray[0][0], sizeof(unsigned int) * 3 * indexArray.size());


	//设置几何物体
	VAO  cubeVAO(CubeVertices, sizeof(CubeVertices), { 3, 3, 2 });
	VAO  quadVAO(quadVertices, sizeof(quadVertices), { 3, 3, 2 }, quadIndices, sizeof(quadIndices));
	VAO	 quadShaderVAO(derferShaderQuad, sizeof(derferShaderQuad), { 3, 3, 2 }, derferShaderQuadIndices, sizeof(derferShaderQuadIndices));


	//加载环境贴图
	Texture  hdrTexture(FileSystem::getPath("resources/textures/hdr/newport_loft.hdr").c_str(), true, true, "equirectangularMap");
	Texture  envCubemap(Texture::ENUM_TYPE_CUBE_MAP, 512, 512);


	// 设置6个观察方向来获取cubemap的各个面
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};


	//创建帧缓冲区
	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


	//将等距柱状投影图转化为cubemap
	equirectangularToCubemapShader.use();
	hdrTexture.Use(0);
	equirectangularToCubemapShader.setInt( hdrTexture.getName(), 0);
	equirectangularToCubemapShader.setMat4("projection", captureProjection);
	glViewport(0, 0, 512, 512); 
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		equirectangularToCubemapShader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap.GetID(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cubeVAO.Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);







	////创建uniform buffer object 
	//unsigned int uboMatrices;
	//glGenBuffers(1, &uboMatrices);
	//glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	//glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	////绑定到绑定点
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);



	//注册处理函数（需要和windows相应绑定处理各种输入）
	auto registerInputOp = new RegisterInput(false);
	registerInputOp->Run();


	//渲染环境配置
	auto initOp = new LambdaOp([]() {
		//开启深度测试
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		////开启混合模式
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendEquation(GL_FUNC_ADD);

		////开启背面剔除
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CCW); // gl_ccw 代表的是逆时针的环绕方式
	}, false);
	initOp->Run();


	//设置shader材质， 环境灯光, 等其他 uniform 变量
	auto  settingEnvir = new LambdaOp([&] {
		pbrShader.use();
		pbrShader.setVec3("albedo", 0.5f, 0.0f, 0.0f);
		pbrShader.setFloat("ao", 1.0f);


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


	//更新ubo矩阵
	glm::mat4   uboMat4Arrays[2];
	auto  updateCameraUbo = new LambdaOp([&]() {
		////更新ubo, 可以采取批量更新也可以逐次更新uniform 
		//glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		//uboMat4Arrays[0] = mainCamera.GetProjectionMatrix();
		//uboMat4Arrays[1] = mainCamera.GetViewMatrix();
		//glBufferSubData(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::mat4), uboMat4Arrays);
		//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	});

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	auto geomtryOp = new  LambdaOp([&]() {


		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		pbrShader.use();
		pbrShader.setMat4("projection", mainCamera.GetProjectionMatrix());
		pbrShader.setMat4("view", mainCamera.GetViewMatrix());
		pbrShader.setVec3("camPos", mainCamera.GetPos());

		//渲染一排不同参数对比的球体
		glm::mat4 model = glm::mat4(1.0f);
		for (int row = 0; row < nrRows; ++row)
		{
			//金属光泽度对比
			pbrShader.setFloat("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; ++col)
			{
				//粗糙度对比
				pbrShader.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(float)(col - (nrColumns / 2)) * spacing,
					(float)(row - (nrRows / 2)) * spacing,
					-2.0f
				));
				pbrShader.setMat4("model", model);
				sphereVAO.Draw();
			}
		}

		//添加灯光参数
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			pbrShader.setMat4("model", model);
			sphereVAO.Draw();
		}


		//渲染天空盒
		backgroundShader.use();
		backgroundShader.setMat4("view", mainCamera.GetViewMatrix());
		backgroundShader.setMat4("projection", mainCamera.GetProjectionMatrix());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap.GetID());
		cubeVAO.Draw();
	});


	//渲染队列
	auto renderQueue = new  OpQueue();
	*renderQueue << updateCameraUbo << geomtryOp;


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

