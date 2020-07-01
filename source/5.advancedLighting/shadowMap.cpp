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
	//创建世界空间到光线空间的变换矩阵（用于生成阴影贴图)

	float ratioWH_2 = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	Camera   lightSpaceCamera(ratioWH_2, moveSpeed, rotateSpeed, lightPos, NEAR_PLANE, FAR_PLANE, up, YAW, PITCH, Camera::ENUM_Projection::PROJECTION_ORTHO);
	lightSpaceCamera.SetFront(glm::normalize(glm::vec3(0.0f) - lightPos));

	////设置几何物体
	VAO  planeVAO(planeVertices, sizeof(planeVertices), { 3, 3, 2 });
	VAO  cubeVAO(CubeVertices, sizeof(CubeVertices), { 3, 3, 2 });

	Shader sceneShadowShader("./shader/advancedLighting/sceneShadow.vs", "./shader/advancedLighting/sceneShadow.fs");
	GStorage<Shader*>::GetInstance()->Register(str_BlinnPhong, &sceneShadowShader);
	sceneShadowShader.BindUniformBlockIndex("Matrices", 0);
	Shader   simplerDepthShader("./shader/advancedLighting/depthMap.vs", "./shader/advancedLighting/depthMap.fs");
	simplerDepthShader.BindUniformBlockIndex("LightSpaceMatrix", 1);

	Texture  texContainer(FileSystem::getPath("resources/textures/container2.png").c_str(), true, false);
	Texture  planeTex(FileSystem::getPath("resources/textures/wood.png").c_str(), true, false);
	planeTex.SetName("texture_diffuse1");

	FBO		depthFbo(SHADOW_WIDTH, SHADOW_HEIGHT, FBO::Enum_Type::ENUM_TYPE_DEPTH);

	//网格物体
	Mesh	planeMesh(planeVAO, planeTex);
	Mesh    cubeMesh(cubeVAO, texContainer);


	//创建uniform buffer object 
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//绑定到绑定点
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMatrices);
	//------------ Light Matrix UBO
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = -10.0f, far_plane = 27.5f;
	//lightProjection = glm::ortho(-8.0f, 8.0f, -6.0f, 6.0f, near_plane, far_plane);
	//lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

	lightProjection = lightSpaceCamera.GetProjectionMatrix();
	lightView = lightSpaceCamera.GetViewMatrix();

	lightSpaceMatrix = lightProjection * lightView;
	unsigned int	lightMatrixUBO;
	glGenBuffers(1, &lightMatrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, lightMatrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix), GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightMatrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);



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
		sceneShadowShader.use();
		//绘制之前都必须设置渲染状态, 设置位置, 物体渲染(如果状态不复位可以放置在渲染循环之前)
		//全局属性面板
		sceneShadowShader.setBool("blinn", true);

		//材质属性
		sceneShadowShader.setFloat("material.shininess", material_shininess);

		// 灯光属性
		sceneShadowShader.setVec3("light.position", lightPos);
		sceneShadowShader.setVec3("light.ambient", light_ambient);
		sceneShadowShader.setVec3("light.diffuse", light_diffuse);
		sceneShadowShader.setVec3("light.specular", light_specular);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//设置深度得调用glViewport。因为阴影贴图经常和我们原来渲染的场景（通常是窗口分辨率）有着不同的分辨率，
		//我们需要改变视口（viewport）的参数以适应阴影贴图的尺寸
		glBindFramebuffer(GL_FRAMEBUFFER, depthFbo.GetID());
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		simplerDepthShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		simplerDepthShader.setVec3("viewPos", mainCamera.GetPos());
		for (auto iter = cubePositions.begin(); iter != cubePositions.end(); ++iter)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, *iter);
			//model = glm::scale(model, glm::vec3(0.5f));
			simplerDepthShader.setMat4("model", model);
			cubeMesh.Draw(simplerDepthShader);
		}
		//绘制地板
		model = glm::mat4(1.0f);
		simplerDepthShader.setMat4("model", model);
		simplerDepthShader.setVec3("viewPos", mainCamera.GetPos());
		planeMesh.Draw(simplerDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);



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


