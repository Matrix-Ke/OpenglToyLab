#pragma once
#include "util/RegisterInput.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfwWrapper.h>
#include <util/GStorage.h>
#include <util/LambdaOp.h>
#include <util/Camera.h>
#include "Defines.h"
#include "util/shader.h"
#include "util/MyDelegate.h"



using namespace Oper;
using namespace OpenGL;
using namespace Define;
using namespace std;
using namespace Delegate;

void RegisterInput::Run()
{
	RegisterMouse();
	RegisterKey();
	PrintInfo();
}

void RegisterInput::RegisterMouse()
{

	auto mainCamera = *GStorage<Camera *>::GetInstance()->GetPtr(str_MainCamera);
	if (mainCamera != nullptr)
	{
		CMultiDelegate<void, double, double, bool >::GetInstance()->Register(MOUSE_MOUVE, newDelegate(mainCamera, &Camera::ProcessMouseMovement));
		CMultiDelegate<void, double>::GetInstance()->Register(MOUSE_SCROLL, newDelegate(mainCamera, &Camera::ProcessMouseScroll));
	}
	else
	{
		throw exception("mainCamera is not create!!!");
	}
}

void RegisterInput::RegisterKey() 
{
	// Close Window
	auto closeWindowOp = new LambdaOp();
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_ESCAPE,
		[]() { Glfw::GetInstance()->CloseWindow(); });

	// Polygon Mode
	//------------ GLFW_KEY_1
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_1,
		[]() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); });
	//------------ GLFW_KEY_2
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_2,
		[]() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); });

	// Projection
	//------------ GLFW_KEY_3
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_3,
		[]() {
		auto mainCamera = *GStorage<Camera *>::GetInstance()->GetPtr(str_MainCamera);
		mainCamera->SetPerspective();
	});
	//------------ GLFW_KEY_4
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_4,
		[]() {
		auto mainCamera = *GStorage<Camera *>::GetInstance()->GetPtr(str_MainCamera);
		mainCamera->SetOrtho();
	});


	// Move
	size_t moveKey[] = { GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_Q, GLFW_KEY_E };
	size_t arrowKey[] = { GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_PAGE_UP, GLFW_KEY_PAGE_DOWN };
	for (size_t i = 0; i < 6; i++) {
		LambdaOp * op = new LambdaOp([=]() {
			auto mainCamera = *GStorage<Camera *>::GetInstance()->GetPtr(str_MainCamera);
			auto deltaTime = **GStorage<float *>::GetInstance()->GetPtr(str_DeltaTime);
			if (mainCamera == nullptr)
			{
				std::cout << "main camera is init failed" << std::endl;
				return;
			}
			cout << "mainCamera->ProcessKeyboard" << endl;
			mainCamera->ProcessKeyboard(Camera::ENUM_Movement(Camera::MOVE_FORWARD + i), deltaTime);
		});

		EventManager::GetInstance()->Register(EventManager::KEYBOARD | moveKey[i],
			op);
		EventManager::GetInstance()->Register(EventManager::KEYBOARD | arrowKey[i],
			op);
	}

	//shader效果开关
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_B, [=]() 
	{
		static bool blinn = true;
		blinn = !blinn;
		auto BlinnPhongShader = *GStorage<Shader *>::GetInstance()->GetPtr(str_BlinnPhong);
		if (BlinnPhongShader)
		{
			BlinnPhongShader->setBool("blinn", blinn);
			printf("INFO: Switch to %s\n", blinn ? "Blinn-Phong" : "Phong");
		}
	});

	//开启全局gamma校正
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_G, [=]() 
	{
		static bool gamma = true;
		gamma = !gamma;
		auto BlinnPhongShader = *GStorage<Shader *>::GetInstance()->GetPtr(str_BlinnPhong);
		if (BlinnPhongShader)
		{
			BlinnPhongShader->setBool("gamma", gamma);
			printf("INFO: Switch to %s\n", gamma ? "gamma  Enable!" : "gamma Disable");
		}
	});
}

void RegisterInput::PrintInfo() 
{
	cout << endl
		<< "* 1. Press '1' to set PolygonMode[FILL]" << endl
		<< "* 2. Press '2' to set PolygonMode[LINE]" << endl
		<< "* 3. Press '3' to set projection[perspective]" << endl
		<< "* 4. Press '4' to set projection[ortho]" << endl
		<< "* 5. Key [WSADQE] and [left,right,up,down,pageup,pagedown] to control the position."
		<< "* 6. Press 'ESC' to close exe" << endl << endl;
}