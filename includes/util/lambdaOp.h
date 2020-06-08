#pragma once

#include "util/operation.h"
#include <functional>

namespace Oper
{
	class LambdaOp : public Operation
	{
	public:
		LambdaOp(const std::function<void()> & op = []() {}, bool IsHold = true);

		void SetOp(const std::function<void()> & op);

		virtual void Run();

	protected:
		virtual ~LambdaOp();

		std::function<void()> op;

	private:
		LambdaOp(const LambdaOp&) = delete;
		LambdaOp&  operator=(const LambdaOp&) = delete;

	};
}