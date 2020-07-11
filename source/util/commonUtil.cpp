#pragma once
#include "util/commonUtil.h"

#include <iostream>
#include <direct.h>




char* getCurrentDir()
{
	char* buffer = _getcwd(NULL, 0);
#ifdef _DEBUG	
	std::cout << "current working dirrectory : "<< buffer << std::endl;
#endif //_DEBUG
	return buffer;
}


