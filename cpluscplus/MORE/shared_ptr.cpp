// shared_ptr constructor example
#include <iostream>
#include <memory>
using namespace std;
struct C {int* data;};

class MyData {
    public:
    MyData() {x=0;};
    void incrementx() {x++;}
    int getValue() { return x;}
    ~MyData() {cout << "My Data would be destroyed \n" << endl;}
    private:
    int x;
};

int testFunc(MyData* myData) {
   shared_ptr <MyData> p1(myData);
   cout << "Use count:" << p1.use_count() << " pointer:" << p1.get() << "value:" << p1.get()->getValue() << endl;
}


int main () {

  MyData *y = new MyData();
  y->incrementx();
  testFunc(y);
   cout << "pointer:" << y << "value:" << y->getValue() << endl;
  std::shared_ptr<int> p1;
  std::shared_ptr<int> p2 (nullptr);
  std::shared_ptr<int> p3 (new int);
  std::shared_ptr<int> p10 =  p3;
  std::shared_ptr<int> p4 (new int, std::default_delete<int>());
  std::shared_ptr<int> p5 (new int, [](int* p){delete p;}, std::allocator<int>());
  std::shared_ptr<int> p6 (p5);
  std::shared_ptr<int> p7 (std::move(p6));
  std::shared_ptr<int> p8 (std::unique_ptr<int>(new int));
  std::shared_ptr<C> obj (new C);
  std::shared_ptr<int> p9 (obj, obj->data);

  std::cout << "use_count:\n";
  std::cout << "p1: " << p1.use_count() << '\n';
  std::cout << "p2: " << p2.use_count() << '\n';
  std::cout << "p3: " << p3.use_count() << '\n';
  std::cout << "p4: " << p4.use_count() << '\n';
  std::cout << "p5: " << p5.use_count() << '\n';
  std::cout << "p6: " << p6.use_count() << '\n';
  std::cout << "p7: " << p7.use_count() << '\n';
  std::cout << "p8: " << p8.use_count() << '\n';
  std::cout << "p9: " << p9.use_count() << '\n';
  std::cout << "p10: " << p10.use_count() << '\n';
  return 0;
}
