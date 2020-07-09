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


int main()
{
	// glfw: initialize and configure
	Glfw::GetInstance()->Init(SCR_WIDTH, SCR_HEIGHT, windowTitle.c_str());
	//Glfw::GetInstance()->LockCursor();


	//注册相机, 窗口
	float ratioWH = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 5.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);


	//////设置几何物体
	VAO  cubeVAO(CubeVertices, sizeof(CubeVertices), { 3, 3, 2 });
	VAO  quadVAO(quadVertices, sizeof(quadVertices), { 3, 3, 2 }, quadIndices, sizeof(quadIndices));

	Texture  texContainer(FileSystem::getPath("resources/textures/container2.png").c_str(), true, false);
	Texture  planeTex(FileSystem::getPath("resources/textures/wood.png").c_str(), true, false);


	Shader shaderGeometryPass("./shader/advancedLighting/g_buffer.vs", "./shader/advancedLighting/g_buffer.fs");
	shaderGeometryPass.BindUniformBlockIndex("Matrices", 0);
	Shader   shaderLightingPass("./shader/advancedLighting/deferred_shading.vs", "./shader/advancedLighting/deferred_shading.fs");
	

	FBO		gBufferFBO(SHADOW_WIDTH, SHADOW_HEIGHT, FBO::Enum_Type::ENUM_TYPE_GBUFFER);

	//网格物体
	Mesh    cubeMesh(cubeVAO, texContainer);


	//几何模型
	Model cyborg(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
	std::vector<glm::vec3> objectPositions = {
		glm::vec3(-0.0, -0.0, -0.0),
		glm::vec3(-3.0, -3.0, -3.0),
		glm::vec3(0.0, -3.0, -3.0),
		glm::vec3(3.0, -3.0, -3.0),
		glm::vec3(-3.0, -3.0, 0.0),
		glm::vec3(0.0, -3.0, 0.0),
		glm::vec3(3.0, -3.0, 0.0),
		glm::vec3(-3.0, -3.0, 3.0),
		glm::vec3(0.0, -3.0, 3.0),
		glm::vec3(3.0, -3.0, 3.0)
	};

	// - Colors
	const GLuint NR_LIGHTS = 32;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	srand(13);
	for (GLuint i = 0; i < NR_LIGHTS; i++)
	{
		// Calculate slightly random offsets
		GLfloat xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		GLfloat yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
		GLfloat zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// Also calculate random color
		GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}


	//创建uniform buffer object 
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//绑定到绑定点
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);



	//注册处理函数（需要和windows相应绑定处理各种输入）
	auto registerInputOp = new RegisterInput(false);
	registerInputOp->Run();


	//渲染环境配置
	auto initOp = new LambdaOp([]() {
		//开启深度测试
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

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


		shaderLightingPass.use();
		// Also send light relevant uniforms
		for (GLuint i = 0; i < lightPositions.size(); i++)
		{
			shaderLightingPass.setVec3("lights[" + std::to_string(i) + "].position", lightPositions[i]);
			shaderLightingPass.setVec3("lights[" + std::to_string(i) + "].color", lightColors[i]);

			// Update attenuation parameters and calculate radius
			const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
			const GLfloat linear = 0.7;
			const GLfloat quadratic = 1.8;
			shaderLightingPass.setFloat("lights[" + std::to_string(i) + "].linear", linear);
			shaderLightingPass.setFloat("lights[" + std::to_string(i) + "].quadratic", quadratic);
		}
		shaderLightingPass.unBind();

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

		//gBufferFBO.Use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		shaderGeometryPass.use();
		//shaderGeometryPass.setMat4("model", model);
		//cyborg.Draw(shaderGeometryPass);
		{
			for (unsigned int i = 0; i < 1; i++)
			{
				//model = glm::mat4();
				//model = glm::translate(model, objectPositions[i]);
				model = scale(model, glm::vec3(0.25));
				shaderGeometryPass.setMat4("model", model);
				cyborg.Draw(shaderGeometryPass);
			}
		}


		////默认缓冲
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		////shader
		//shaderLightingPass.use();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, gBufferFBO.GetColorTexture(0).GetID());
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, gBufferFBO.GetColorTexture(1).GetID());
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, gBufferFBO.GetColorTexture(2).GetID());
		//shaderLightingPass.setVec3("viewPos", mainCamera.GetPos());

		////用于渲染阴影贴图
		//quadVAO.Draw();

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


