//#include "util/MyDelegate.h"  
//using namespace Delegate;
//
//int NormalFunc(int a)
//{
//	printf("这里是普通函数 ：%d\n", a);
//	return 1;
//}
//
//class A
//{
//public:
//	static int StaticFunc(int a)
//	{
//		printf("这里是成员静态函数 ： %d\n", a);
//		return 1;
//	}
//	int MemberFunc(int a)
//	{
//		printf("这里是成员非静态函数 ： %d\n", a);
//		return 2;
//	}
//};
//
//using namespace Delegate;
//int main()
//{
//	//首先创建了一个返回值为 void ,参数为int 的一个委托。
//	CMultiDelegate<int, int>*   insPtr = CMultiDelegate<int, int>::GetInstance();
//	insPtr->Register(KEYBOARD_PRESS, newDelegate(NormalFunc));
//	insPtr->Register(KEYBOARD_PRESS, newDelegate(&A(), &A::MemberFunc));
//	insPtr->Register(KEYBOARD_PRESS, newDelegate(A::StaticFunc));
//
//	CMultiDelegate<int, int>::GetInstance()->Response(KEYBOARD_PRESS, 7);
//
//
//	return 0;
//}
//
