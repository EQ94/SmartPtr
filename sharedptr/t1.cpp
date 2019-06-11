#include <iostream>
#include <memory>
using namespace std;

int main(){
    int a = 10;
    shared_ptr<int> ptra = make_shared<int>(a);
    shared_ptr<int> ptra2(ptra); // copy
    cout<<ptra.use_count() <<endl;

    int b = 20;
    int *pb=&a;
    shared_ptr<int> ptrb = make_shared<int>(b);
    ptra2 = ptrb;  // assign
    pb = ptrb.get(); // 获取原始指针

    cout<< ptra.use_count() << endl;
    cout<< ptrb.use_count() <<endl;
    
    
    return 0;
}
