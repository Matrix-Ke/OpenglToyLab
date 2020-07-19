#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"


namespace Define 
{
	//阴影贴图
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	const float NEAR_PLANE = 1.0f;
	const float FAR_PLANE = 7.5f;
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float ZOOM = 45.0f;

	// 相机相关设置
	const std::string str_MainCamera = "MainCamera";
	//const unsigned int SCR_WIDTH = 1024;
	//const unsigned int SCR_HEIGHT = 1024;
	const unsigned int SCR_WIDTH = 1280;
	const unsigned int SCR_HEIGHT = 720;
	//const unsigned int SCR_WIDTH = 800;
	//const unsigned int SCR_HEIGHT = 600;
	const float ratioWH = (float)SCR_WIDTH / (float)SCR_HEIGHT;

	const std::string  str_DeltaTime = "DeltaTime";
	const std::string  windowTitle = "windowTitle";
	const std::string  str_BlinnPhong = "str_BlinnPhong";


	//# 相机移动速度, 参考值 10
	const float moveSpeed = 20;
	//# 相机旋转速度, 参考值 0.02
	const float rotateSpeed = 0.02;


	//灯光相关
	const glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
	const glm::vec3 light_ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	const glm::vec3 light_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 light_specular = glm::vec3(0.5f, 0.5f, 0.5f);

	//材质相关
	const float material_shininess = 64.0f;	


	//awesome face quadvertices
	const float quadVertices[] = {
		// 3  ----->  0
		// |		  |
		// V		  V
		// 2  ----->  1
		//---------------
		// positions          // colors           // texture coords
		 1.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f,   -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		 -1.0f,  -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		 -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	//awesome face quad index
	const unsigned int quadIndices[] = {
		1, 0, 3,// first triangle
		1, 3, 2  // second triangle
	};

	//derferShaderQuadShowing
	const float derferShaderQuad[] = {
		// 3  ----->  0
		// |		  |
		// V		  V
		// 2  ----->  1
		//---------------
		// positions								// colors           // texture coords
		 -0.5f,   -0.5f ,  0.0f,		1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 -0.5f,   -1.0f,   0.0f,	    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		 -1.0f,	  -1.0f,   0.0f,		0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		 -1.0f,   -0.5f ,  0.0f,	    1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	//derferShaderQuadShowing index
	const unsigned int derferShaderQuadIndices[] = {
		1, 0, 3,// first triangle
		1, 3, 2  // second triangle
	};

	
	// ------------------------------------------------------------------
	const float CubeVertices[] = {
			// positions          // normals           // texture coords
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left    
	};

	// world space positions of our cubes
	const std::vector<glm::vec3>  cubePositions{
		glm::vec3(0.0f, 1.5f, 0.0),
		glm::vec3(2.0f, 0.0f, 1.0),
		glm::vec3(-1.0f, 0.0f, 2.0),
		//glm::vec3(-3.8f, -2.0f, -12.3f),
		//glm::vec3(2.4f, -0.4f, -3.5f),
		//glm::vec3(-1.7f,  3.0f, -7.5f),
		//glm::vec3(1.3f, -2.0f, -2.5f),
		//glm::vec3(1.5f,  2.0f, -2.5f),
		//glm::vec3(1.5f,  0.2f, -1.5f),
		//glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	const std::vector<glm::vec3> objectPositions = {
		glm::vec3(-2.0, -2.0, -2.0),
		glm::vec3(0.0, -2.0, -2.0),
		glm::vec3(2.0, -2.0, -2.0),
		glm::vec3(-2.0, -2.0, 0.0),
		glm::vec3(0.0, -2.0, 0.0),
		glm::vec3(2.0, -2.0, 0.0),
		glm::vec3(-2.0, -2.0, 2.0),
		glm::vec3(0.0, -2.0, 2.0),
		glm::vec3(2.0, -2.0, 2.0),
		//---------------------------
		glm::vec3(2.0, 0.0, 2.0),
		glm::vec3(-2.0, 0.0, -2.0),
		glm::vec3(0.0, 0.0, -2.0),
		glm::vec3(2.0, 0.0, -2.0),
		glm::vec3(-2.0, 0.0, 0.0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(2.0, 0.0, 0.0),
		glm::vec3(-2.0, 0.0, 2.0),
		glm::vec3(0.0, 0.0, 2.0),
		glm::vec3(2.0, 0.0, 2.0)
	};

	const float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};


	const float pyramidVertices[] = {
		//position
		1.0, 0.0, 1.0,	
		0.0, 0.0, -1.0, 
		-1.0, 0.0, 1.0, 
		0.0, 1.0, 0.0
	};

	const unsigned int pyramidIndices[] = {
		0, 1, 2, // first triangle
		0, 1, 3,
		1, 2, 3,
		2, 0, 3
	};


	//地面四边形数据设定
	const float planeVertices[] = {
		// positions          // normals         // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		// 2  ----->  1
		// |		  |
		// V		  V
		// 3  ----->  0
		//-------------- - index   0->1->2    0->2->3 逆时针
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  2.0f, 0.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  2.0f, 2.0f,
		 -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f,   0.0f, 2.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  2.0f, 0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  0.0f, 2.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f

		//		// positions            // normals         // texcoords
		// 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		//-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		//-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		// 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		//-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		// 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f

	};


	const float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  1.0f,  0.0f,  1.0f,
		1.5f, -0.5f,  1.0f,  1.0f,  1.0f,

		0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		1.5f, -0.5f,  1.0f,  1.0f,  1.0f,
		1.5f,  0.5f,  1.0f,  1.0f,  0.0f
	};
	//半透明框的位移矩阵
	const std::vector<glm::vec3>		Translation
	{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};
}
