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
 	this_thread::sleep_for(chrono::seconds(1));  // chrono::seconds(ref_time):�ӵ�ǰʱ�俪ʼ������ǰ�߳�֪��rel_time(һ��ʱ��)��ȥ�������߳�����ִ��
	shared_ptr<Base> lp = p;
	{	// ������ָ���˱������������ڴ������������ľֲ��������������Ա���������ʼ����������֮�������
		// ע�⣬�����ڴ������ڶ���߱������г�ʼ�������߸�ֵ��������Ȼ��Ӱ����߱���������
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