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
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, -2.0f, 5.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);


	//设置几何物体, 创建球体网格
	Sphere   baseSphere(100);
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

	VAO  sphereVAO(&sphereVertices[0], sizeof(float) * sphereVertices.size(), { 3, 3, 2 }, &indexArray[0][0], sizeof(int) * 3 * indexArray.size());

	VAO  quadVAO(quadVertices, sizeof(quadVertices), { 3, 3, 2 }, quadIndices, sizeof(quadIndices));
	VAO	 quadShaderVAO(derferShaderQuad, sizeof(derferShaderQuad), { 3, 3, 2 }, derferShaderQuadIndices, sizeof(derferShaderQuadIndices));

	Shader   pbrShaderPass("./pbrShaderPass/pbrShader/pbrLighting.vs", "./pbrShaderPass/pbrShader/pbrLighting.fs");
	pbrShaderPass.BindUniformBlockIndex("Matrices", 0);
	//Shader   shaderQuadShowPass("./pbrShaderPass/advancedLighting/derferShaderShowing.vs", "./pbrShaderPass/advancedLighting/derferShaderShowing.fs");
	//Shader   shaderQuadShowSpePass("./pbrShaderPass/advancedLighting/derferShaderShowing_1.vs", "./pbrShaderPass/advancedLighting/derferShaderShowing_1.fs");




	//创建uniform buffer object 
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//绑定到绑定点
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// lights
	std::vector<glm::vec3> lightPositions = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	std::vector<glm::vec3> lightColors = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

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

		//开启背面剔除
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW); // gl_ccw 代表的是逆时针的环绕方式
	}, false);
	initOp->Run();


	//设置pbrShaderPass材质， 环境灯光, 等其他 uniform 变量
	auto  settingEnvir = new LambdaOp([&] {
		//shaderGeometryPass.use();
		//绘制之前都必须设置渲染状态, 设置位置, 物体渲染(如果状态不复位可以放置在渲染循环之前)
		//全局属性面板
		//shaderGeometryPass.setBool("blinn", true);

		////材质属性
		//shaderGeometryPass.setFloat("material.shininess", material_shininess);

		//// 灯光属性
		//shaderGeometryPass.setVec3("light.position", lightPos);
		//shaderGeometryPass.setVec3("light.ambient", light_ambient);
		//shaderGeometryPass.setVec3("light.diffuse", light_diffuse);
		//shaderGeometryPass.setVec3("light.specular", light_specular);

		//shaderGeometryPass.setFloat("near_plane", NEAR_PLANE);
		//shaderGeometryPass.setFloat("far_plane", FAR_PLANE);
		//shaderGeometryPass.unBind();


		pbrShaderPass.use();
		// Also send light relevant uniforms
		for (GLuint i = 0; i < lightPositions.size(); i++)
		{
			pbrShaderPass.setVec3("lights[" + std::to_string(i) + "].position", lightPositions[i]);
			pbrShaderPass.setVec3("lights[" + std::to_string(i) + "].color", lightColors[i]);

			// Update attenuation parameters and calculate radius
			const GLfloat constant = 1.0; // Note that we don't send this to the pbrShaderPass, we assume it is always 1.0 (in our case)
			const GLfloat linear = 0.7;
			const GLfloat quadratic = 1.8;
			pbrShaderPass.setFloat("lights[" + std::to_string(i) + "].linear", linear);
			pbrShaderPass.setFloat("lights[" + std::to_string(i) + "].quadratic", quadratic);
		}
		pbrShaderPass.unBind();

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


	//更新相机view 和 projection 矩阵
	glm::mat4   uboMat4Arrays[2];
	auto  updateCameraUbo = new LambdaOp([&]() {
		//更新ubo, 可以采取批量更新也可以逐次更新uniform 
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		uboMat4Arrays[0] = mainCamera.GetProjectionMatrix();
		uboMat4Arrays[1] = mainCamera.GetViewMatrix();
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 2 * sizeof(glm::mat4), uboMat4Arrays);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	});


	auto geomtryOp = new  LambdaOp([&]() {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);


		pbrShaderPass.setVec3("viewPos", mainCamera.GetPos());

		// render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
		glm::mat4 model = glm::mat4(1.0f);
		for (int row = 0; row < nrRows; ++row)
		{
			pbrShaderPass.setFloat("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; ++col)
			{
				// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
				// on direct lighting.
				pbrShaderPass.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(col - (nrColumns / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f
				));
				pbrShaderPass.setMat4("model", model);
				sphereVAO.Draw();
			}
		}

		// render light source (simply re-render sphere at light positions)
		// this looks a bit off as we use the same pbrShaderPass, but it'll make their positions obvious and keeps the codeprint small.
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			pbrShaderPass.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			pbrShaderPass.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			pbrShaderPass.setMat4("model", model);
			sphereVAO.Draw();
		}
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


