#include<iostream>
#include<memory>
using namespace std;
int main()
{
	{
		shared_ptr<int> sh_ptr = make_shared<int>(10);
		cout<< sh_ptr.use_count() <<endl;	 // 1
		cout<< sh_ptr.get() << endl;		 // 0xf95020

		weak_ptr<int> wp(sh_ptr);
		cout<<wp.use_count() <<endl;         // 1
		cout<<sh_ptr.get() << endl;          // 0xf95020

		if(!wp.expired()){
			shared_ptr<int> sh_ptr2 = wp.lock();  // get another shared_ptr
			cout<<wp.use_count() <<endl;     // 2
			cout<<sh_ptr.use_count() <<endl; // 2 
			cout<<sh_ptr.get() <<endl;       // 0xf95020

			*sh_ptr = 100;
			cout<<wp.use_count() <<endl;     // 2
			cout<<sh_ptr.use_count() <<endl; // 2
			cout<<sh_ptr.get() <<endl;       // 0xf95020
		}
	}
	return 0;
}
