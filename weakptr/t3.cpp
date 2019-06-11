// weak_ptr  循环引用
// weak_ptr并没有重载operator->和operator*操作符，因此不可直接通过weak_ptr使用对象
// 典型的永达就是调用其lock函数，来获得shared_ptr示例，进而访问原始对象。
#include<iostream>
#include<memory>
using namespace std;
class ClassB;
class ClassA{
public:
	ClassA(){ cout<< "ClassA Constructor ..." <<endl;}
	~ClassA(){ cout<< "ClassA Destructor..."<<endl; }
	weak_ptr<ClassB> pb;   // 在类A中引用B
};

class ClassB{
public:
	ClassB(){ cout<<"ClassB Constructor..."<<endl; }
	~ClassB(){ cout<<"ClassB Destructor..."<<endl; }
	weak_ptr<ClassA> pa; // 在类B中引用A
};
int main()
{
	shared_ptr<ClassA> spa = make_shared<ClassA>();  // ClassA Construct
	shared_ptr<ClassB> spb = make_shared<ClassB>();  // ClassB Construct

	spa->pb = spb;
	spb->pa = spa;

	return 0;  
                   // ClassB Destructor
                   // ClassA Destructor
}   
