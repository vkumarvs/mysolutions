#include <stdio.h>
#include <iostream>

class CommSubscriber                                                                                                  
{   
public:
    CommSubscriber(int x, int y):                                                 
        m_x(x),                                                                                     
        m_y(y)
        {                                                                                                             
        }                                                                                                             
    void operator()(const int z) const;                                                            
    
private:                                                                                                              
    int m_x;                                                                                     
    int m_y;
};         

void CommSubscriber::operator()(const int z) const    
{

printf("hello my name is vipin\n");

}

int main()
{
CommSubscriber t(10,20);
t(30);
}          
