#pragma once
#include  <iostream>
#include  <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"


//using namespace std;

GLenum glCheckError_(const char *file, int line);

#define glCheckError() glCheckError_(__FILE__, __LINE__) 



void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,  GLenum severity,  
							GLsizei length,  const GLchar *message, const void *userParam);