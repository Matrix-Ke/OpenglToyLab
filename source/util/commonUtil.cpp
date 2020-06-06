#pragma once
#include "util/commonUtil.h"

#include <iostream>
#include <direct.h>




char* getCurrentDir()
{
	char* buffer = _getcwd(NULL, 0);
	std::cout << buffer << std::endl;
	return buffer;
}


