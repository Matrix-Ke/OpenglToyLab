//#include "util/MyDelegate.h"  
//using namespace Delegate;
//
//
//void foofunc(int)
//{
//	printf("fooFunc test");
//}
//
//int NormalFunc(int a)
//{
//	printf("这里是普通函数 ：%d\n", a);
//	return 1;
//}
//
//template<typename... params>
//class A
//{
//public:
//	typedef void(*func)(params...);
//	static int StaticFunc(int a)
//	{
//		printf("这里是成员静态函数 ： %d\n", a);
//		return 1;
//	}
//	int MemberFunc(params...)
//	{
//		printf("tesat  ih\n");
//		return 2;
//	}
//private:
//	func mFunc;
//};
//
//using namespace Delegate;
//
//typedef void(*func_ptr)(int);
//
//int main()
//{
//	A<int>   fooA;
//	fooA.MemberFunc(3);
//	A<void>  foob;
//	foob.MemberFunc();
//	
//
//
//	return 0;
//}
////
