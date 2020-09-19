#include "util/MyDelegate.h"  
using namespace Delegate;

void NormalFunc(int a)
{
	printf("这里是普通函数 ：%d\n", a);
}

class A
{
public:
	static void StaticFunc(int a)
	{
		printf("这里是成员静态函数 ： %d\n", a);
	}
	void MemberFunc(int a)
	{
		printf("这里是成员非静态函数 ： %d\n", a);
	}
};

int main()
{
	//首先创建了一个返回值为 void ,参数为int 的一个委托。  
	CMultiDelegate<void, int> e;

	//将三个函数注册到该委托中  
	e += newDelegate(NormalFunc);
	e += newDelegate(A::StaticFunc);
	e += newDelegate(&A(), &A::MemberFunc);



	return 0;
}