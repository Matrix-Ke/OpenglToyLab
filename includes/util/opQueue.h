#pragma once
#include <list>
#include "util/operation.h"


namespace Oper
{
	class OpQueue : public Operation
	{
	public:
		OpQueue(bool isHold = true);

		//------------
		OpQueue & operator<<(const Ptr<Operation> & operation);
		OpQueue & operator<<(Operation * operation);
		void Push(Ptr<Operation> & op);
		void Push(Operation * op);
		size_t Size() const;
		//------------
		virtual void Run();

	protected:
		std::list< Ptr<Operation>>  m_OpList;
		virtual   ~OpQueue();

	private:

		OpQueue(const OpQueue &) = delete;
		OpQueue&  operator = (const Operation&) = delete;
	};
}
