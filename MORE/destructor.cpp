#include <cstdio>

/******************************/

// Base class
struct A
{
    A() { printf( "\tInstance counter = %d (ctor)\n", ++instance_counter ); }
    //virtual ~A() { printf( "\tInstance counter = %d (dtor)\n", --instance_counter ); }
    ~A() { printf( "\tInstance counter = %d (dtor)\n", --instance_counter ); }

    static int instance_counter;
};

// Inherited class with default ctor/dtor
class B : public A
{
    public:
    B() { printf("\tB says hi!\n"); }
    ~B() { printf("\tB says bye!\n"); }
};

// Inherited class with defined ctor/dtor
struct C : public A
{
    C() { printf("\tC says hi!\n"); }
    ~C() { printf("\tC says bye!\n"); }
};

/******************************/

// Initialize counter
int A::instance_counter = 0;

/******************************/

// A few tests
//
int main()
{
#if 1
    printf("Create A\n"); A a;
    printf("Delete A\n"); a.~A();

#endif
    printf("Create B\n"); B b;
    printf("Delete B\n"); b.~B();
    printf("Create new B stored as A*\n"); A *a_ptr = new B();
    printf("Delete previous pointer\n"); delete a_ptr;
#if 1
    printf("Create C\n"); C c;
  printf("Delete C\n"); c.~C();
#endif
}
