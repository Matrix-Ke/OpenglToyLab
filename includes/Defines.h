#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"


namespace Define {
	const std::string str_MainCamera = "MainCamera";

	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	const std::string  str_DeltaTime = "DeltaTime";
	const std::string  windowTitle = "windowTitle";

	//# 相机移动速度, 参考值 10
	const float moveSpeed = 20;
	//# 相机旋转速度, 参考值 0.02
	const float rotateSpeed = 0.02;


	//灯光相关
	const glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	const glm::vec3 light_ambient = glm::vec3(0.6f, 0.6f, 0.6f);
	const glm::vec3 light_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	const glm::vec3 light_specular = glm::vec3(1.0f, 1.0f, 1.0f);

	//材质相关
	const float material_shininess = 64.0f;	
	
	// ------------------------------------------------------------------
	const float CubeVertices[] = {
	// positions          // normals           // texture coords
    // Back face		
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 	0.0f, 0.0f, // Bottom-left
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 	1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 	1.0f, 0.0f, // bottom-right 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 	1.0f, 1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 	0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 	0.0f, 1.0f, // top-left
    // Front face                             	
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 	0.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 	1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 	1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 	1.0f, 1.0f, // top-right
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 	0.0f, 1.0f, // top-left
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 	0.0f, 0.0f, // bottom-left
    // Left face                              	
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 	1.0f, 0.0f, // top-right
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 	1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 	0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 	0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 	0.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 	1.0f, 0.0f, // top-right
    // Right face                             	
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 	1.0f, 0.0f, // top-left
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 	0.0f, 1.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 	1.0f, 1.0f, // top-right    
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 	0.0f, 1.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 	1.0f, 0.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 	0.0f, 0.0f, // bottom-left  
    // Bottom face                            	
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 	0.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 	1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 	1.0f, 0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 	1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 	0.0f, 0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 	0.0f, 1.0f, // top-right
    // Top face                               	
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 	0.0f, 1.0f, // top-left
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 	1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 	1.0f, 1.0f, // top-right    
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 	1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 	0.0f, 1.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 	0.0f, 0.0f  // bottom-left  
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


	// world space positions of our cubes
	const glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//awesome face quadvertices
	const float quadVertices[] = {
		// 3  ----->  0
		// |		  |
		// V		  V
		// 2  ----->  1
		//---------------
		// positions          // colors           // texture coords
		 1.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 1.0f,   0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		 0.0f,   1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	//awesome face quad index
	const unsigned int quadIndices[] = {
		1, 0, 3,// first triangle
		1, 3, 2  // second triangle
	};


	//地面四边形数据设定
	const float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f,
		 -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f

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

	const std::vector<glm::vec3>		Translation
	{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};
}
