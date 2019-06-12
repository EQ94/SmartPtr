#include<iostream>
#include<memory>
#include<thread>
#include<chrono>
#include<mutex>
using namespace std;
struct Base{
	Base() { cout << "Base::Base()" << endl; };
	~Base() { cout << "Base::~Base()" << endl; };
};
struct Derived :public Base {
	Derived() { cout << "Derived::Derived()" << endl; };
	~Derived() { cout << "Derived::~Derived()" << endl; };
};
void thr(shared_ptr<Base> p) {
 	this_thread::sleep_for(chrono::seconds(1));  // chrono::seconds(ref_time):从当前时间开始阻塞当前线程知道rel_time(一段时间)过去，其他线程依旧执行
	shared_ptr<Base> lp = p;
	{	// 大括号指明了变量的作用域，在大括号内声明的局部变量其作用域自变量声明开始，到大括号之后结束。
		// 注意，但是在大括号内对外边变量进行初始化，或者赋值操作，依然会影响外边变量的内容
		static mutex io_mutex;
		lock_guard<mutex> lk(io_mutex);
		cout << "local pointer in a thread: " << endl;
		cout << " lp.get() = " << lp.get() << endl;
		cout << ", lp.get() = " << lp.use_count() << endl;
	}
}
int main() {
	shared_ptr<Base> p = make_shared<Derived>();
	cout << "Created a shared Derived (as a pointer to Base)" << endl;
	cout << "p.get() = " << p.get();
	cout << ", p.use_count() = " << p.use_count() << endl;

	thread t1(thr, p);
 	thread t2(thr, p);

	p.reset(); // release onwership from main

	cout << "Shared ownership between 2 threads and released" << endl;
	cout << "ownership from main : " << endl;
	cout << " p.get() = " << p.get();
	cout << ", p.use_count() = " << p.use_count();

	t1.join();
 	t2.join();

	cout << "All threads completed, the last one deleted Derived" << endl;

	system("pause");
	return 0;
}