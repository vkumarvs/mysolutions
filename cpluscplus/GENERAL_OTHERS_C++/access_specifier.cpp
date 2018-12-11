class MyClass
{
    public:
        int a;
    protected:
        int b;
    private:
        int c;
};
class inher:public MyClass
{
public:
    int t;
    void display()
    {
      //c = 10;
      b = 30;
    }
};

int main()
{
    MyClass obj;
    inher cla;
    obj.a = 10;     //Allowed
    //obj.b = 20;     //Not Allowed, gives compiler error
 //   obj.c = 30;     //Not Allowed, gives compiler error
    //cla.b = 30;
}

