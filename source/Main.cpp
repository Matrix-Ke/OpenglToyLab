#include "util/MyDelegate.h"  
using namespace Delegate;

void NormalFunc(int a)
{
	printf("��������ͨ���� ��%d\n", a);
}

class A
{
public:
	static void StaticFunc(int a)
	{
		printf("�����ǳ�Ա��̬���� �� %d\n", a);
	}
	void MemberFunc(int a)
	{
		printf("�����ǳ�Ա�Ǿ�̬���� �� %d\n", a);
	}
};

int main()
{
	//���ȴ�����һ������ֵΪ void ,����Ϊint ��һ��ί�С�  
	CMultiDelegate<void, int> e;

	//����������ע�ᵽ��ί����  
	e += newDelegate(NormalFunc);
	e += newDelegate(A::StaticFunc);
	e += newDelegate(&A(), &A::MemberFunc);



	return 0;
}