//#include "util/MyDelegate.h"  
//using namespace Delegate;
//
//int NormalFunc(int a)
//{
//	printf("��������ͨ���� ��%d\n", a);
//	return 1;
//}
//
//class A
//{
//public:
//	static int StaticFunc(int a)
//	{
//		printf("�����ǳ�Ա��̬���� �� %d\n", a);
//		return 1;
//	}
//	int MemberFunc(int a)
//	{
//		printf("�����ǳ�Ա�Ǿ�̬���� �� %d\n", a);
//		return 2;
//	}
//};
//
//using namespace Delegate;
//int main()
//{
//	//���ȴ�����һ������ֵΪ void ,����Ϊint ��һ��ί�С�
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
