#include<iostream>
#include<memory>
#include<vector>
#include<cstdio>
#include<fstream>
#include<cassert>
#include<functional>

using namespace std;

struct B{
	virtual void bar(){cout<<"B::bar"<<endl;}
	virtual ~B() = default;
};
struct D:B
{
	D(){cout<<"D::D"<<endl;}
	~D(){cout<<"D::~D"<<endl;}
	void bar() override{cout<<"D::bar"<<endl;}
};

// a function consuming a unique_ptr can take it by value or by rvalue reference
unique_ptr<D> pass_through( unique_ptr<D> p)
{
	p->bar();
	return p;
}

void close_file(FILE* fp)
{
	fclose(fp);
}

int main()
{
	cout<<"unique onwership semantics demo"<<endl;
	{
		auto p = make_unique<D>();  // p is a unique_ptr that owns a D
		auto q = pass_through( move(p) );
		assert(!p);  // now p owns nothing and holds a null pointer
		q->bar();    // and q owns the D object
	} 	// ~D called here
	
	cout<<" Runtime polymorphism demo  "<<endl;
	{
		unique_ptr<B> p = make_unique<D>();  // p is a unique_ptr that owns a D as a pointer to base
		
		p->bar();  // virtual dispatch

		vector<unique_ptr<B>> v;  // unique_ptr can be stored in a container
		v.push_back(make_unique<D>());
		v.push_back(move(p));
		v.emplace_back(new D);
		for(auto & p:v)
			p->bar();   // virtual dispatch		
	}  // ~D called 3 times

	cout<<"Custom deleter demo"<<endl;
	ofstream("demo.txt") << 'x';  // prepare the file to read
	{
		unique_ptr<FILE, decltype(&close_file) > fp(fopen("demo.txt", "r"), &close_file);
		if(fp) // fopen could have failed ; in which case fp holds a null pointer
			cout<<(char)fgetc(fp.get())<<endl;
	}  // fclose called here , but only if FILE* is not a null pointer ( that is ,if fopen succeeded)

	cout<<"Custom lambda-expression deleter demo"<<endl;
	{
		unique_ptr<D, function<void(D*)>> p (new D, [](D* ptr)
			{
				cout<<"destroying from a curtom deleter ..."<<endl;
				delete ptr;
			}); // p owns D
		p->bar();
	}  // the lambda above is called and D is destroyed

	cout<<"Array form of unique_ptr demo"<<endl;
	{
		unique_ptr<D[]> p{new D[3]};
	}  // calls ~D 3 times

	return 0;


}



