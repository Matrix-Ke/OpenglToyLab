#include "util/opQueue.h"

#include <vector>

using namespace Oper;
using namespace std;

OpQueue::OpQueue(bool isHold)
	: Operation(isHold) { }

OpQueue::~OpQueue() { printf("INFO: Delete OpQueue\n"); }

//------------

OpQueue & OpQueue::operator<<(const Ptr<Operation> & op) {
	if (op != nullptr)
		this->m_OpList.push_back(op);
	return *this;
}

OpQueue & OpQueue::operator<<(Operation * op) {
	if (op != nullptr)
		this->m_OpList.push_back(Operation::ToPtr(op));
	return *this;
}

void OpQueue::Push(Ptr<Operation> & op) {
	if (op != nullptr)
		this->m_OpList.push_back(op);
}

void OpQueue::Push(Operation * op) {
	if (op != nullptr)
		this->m_OpList.push_back(Operation::ToPtr(op));
}
size_t OpQueue::Size() const {
	return this->m_OpList.size();
}

void OpQueue::Run() {
	vector< list<Ptr<Operation>>::const_iterator > removeIt;
	for (auto it = this->m_OpList.cbegin(); it != this->m_OpList.cend(); ++it) {
		(*it)->Run();
		if (!(*it)->IsHold())
			removeIt.push_back(it);
	}
	for (int i = removeIt.size() - 1; i >= 0; i--)
		this->m_OpList.erase(removeIt[i]);
}