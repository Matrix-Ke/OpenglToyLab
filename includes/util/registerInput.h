#pragma once

#include "util/Operation.h"

class RegisterInput : public Oper::Operation {
public:
	RegisterInput(bool isHold = true)
		:Operation(isHold) { }
	void Run();
private:
	void RegisterMouse();
	void RegisterKey();
	void PrintInfo();
};


