#pragma once
#include "util/operation.h"

using namespace Oper;
using namespace std;


Oper::Operation::Operation(bool IsHold /*= true*/): m_isHold(IsHold) {};

Oper::Operation::~Operation() {};

bool Oper::Operation::IsHold()
{
	return m_isHold;
}

void Oper::Operation::SetIsHold(bool ishold)
{
	this->m_isHold = ishold;
}

void Oper::Operation::operator()()
{
	this->Run();
}

void Oper::Operation::ProtectDelete(Operation* op)
{
	delete op;
}