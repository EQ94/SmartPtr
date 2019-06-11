#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std;
struct Base{
	Base(){ cout<<"Base::Base()\n";  }
	~Base(){ cout<<"Base::~Base() \n"; }
};

struct Derived: public Base{
    Derived(){ cout<<"Derived::Devried()\n"; }
	~Derived(){ cout<<"Derived::~Derived()\n"; }
};
void thr(shared_ptr<Base> p){
	this_thread::sleep_for(chrono::seconds(1));
	shared_ptr<Base> lp = p;
	{   // 大括号指明了变量的作用域，在大括号内声明的局部变量其作用域自变量声明始，到大括号之后终结。
		static mutex io_mutex;
		lock_guard<mutex> lk(io_mutex);
		cout<<"local pointer in a thread: \n "
			<<"  lp.get() = "<< lp.get()
			<<", lp.use_count() = "<<lp.use_count()<<'\n'; 
	}
}
int main(){
	shared_ptr<Base> p = make_shared<Derived>();
	cout<<"Created a shared Derived (as a pointer to Base)\n"
		<<" p.get() = "<<p.get()
		<<", p.use_count() = "<<p.use_count() << '\n';
	std::thread t1(thr,p);
	std::thread t2(thr,p);
	std::thread t3(thr,p);
 	p.reset(); // release ownership from main
	cout<<"Shared ownership between 2 threads and released\n"
		<<"ownership from main: \n"
		<<" p.get() = "<<p.get()
		<<",p.use_count() = "<< p.use_count()<<'\n';
	t1.join();
	t2.join();
	t3.join();
	cout<<"All threads completed , the last one deleted Derived\n";
	return 0;
}
