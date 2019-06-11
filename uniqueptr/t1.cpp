#include <iostream>
#include <memory>

using namespace std;

int main()
{
	{
	unique_ptr<int> uptr(new int(10)) ;  // 绑定动态对象
	// unique_ptr<int> uptr2 = uptr ;    // 不能赋值
	// unique_ptr<int> uptr2(uptr) ;     // 不能拷贝
	unique_ptr<int> uptr2 = move(uptr);   // 转移所有权
	uptr2.release(); // 释放所有权
	}  
	// 超过uptr作用域，内存释放
	return 0;
}
