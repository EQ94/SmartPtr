#include<iostream>
#include<memory>
using namespace std;

class A
{
public:
	A():a(3) {cout<<"A Constructor..."<<endl; }
	~A() {cout<<"A Destructor..."<<endl;}
	int a;
};
int main()
{	
	shared_ptr<A> sp(new A() );
	weak_ptr<A> wp(sp);
	//sp.reset();

	if(shared_ptr<A> pa =wp.lock() )
	{
		cout<< pa->a <<endl;
	}
	else
	{
		cout<< "wp指向对象为空" <<endl;
	}
	
}
