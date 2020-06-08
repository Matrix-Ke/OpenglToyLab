#pragma once

#include <string>

namespace Oper
{
	template<typename T> 
	using	Ptr = std::shared_ptr<T>;

	class Operation
	{
	public:
		template <typename T> 
		static Ptr<T> ToPtr(T* op)
		{
			return Ptr<T>(op, ProtectDelete);
		}

		bool IsHold();

		void SetIsHold(bool ishold);

		void operator()();

		virtual void Run() = 0;

	protected:

		Operation(bool IsHold = true);

		static  void  ProtectDelete(Operation* op);

		virtual ~Operation();

		bool m_isHold;

	private:
		Operation(const Operation&) = delete;

		Operation&  operator=(const Operation&) = delete;

	};
}