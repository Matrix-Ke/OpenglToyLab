#pragma once

#include "util/lambdaOp.h"

#include <iostream>

using namespace Oper;

void Oper::LambdaOp::SetOp(const std::function<void()> & op)
{
	this->op = op;
}

void Oper::LambdaOp::Run()
{
	if (op == nullptr)
	{
		m_isHold = false;
	}
	else
	{
		op();
	}
}

Oper::LambdaOp::~LambdaOp()
{
	std::cout << "Info : delete lambdaOp\n";
}


LambdaOp::LambdaOp(const std::function<void()> & op, bool isHold)
	: op(op), Operation(isHold) { }
