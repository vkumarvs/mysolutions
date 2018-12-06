#include <iostream>
#include <list>
#include <map>
using namespace std;
struct test
{
    const char* last;
};
struct CommandOp 
{
    uint8_t api_id;
    uint8_t commandId;
    uint8_t catagory; 
    uint8_t transId;
    uint8_t prevTransId;
    uint8_t status;
    int thandle; /*Transaction id to communicate to confd */
    void *command_elem;
 /*   ~CommandOp()
    {
     free(command_elem);
    };*/
};
int main()
{
    CommandOp x,y,z;
    x.api_id = 10;
    y.api_id = 11;
    z.api_id = 12;
    test *test1 = new test;
    test1->last = "dahiya";
    const char *name = "vipin";
    CommandOp *z1 = new CommandOp();
    CommandOp *z2 = new CommandOp();
    CommandOp *z3 = new CommandOp();
    z1->command_elem = (void *)test1;
    z2->command_elem = (void *)test1;
    z3->command_elem = (void *)test1;
    z1->api_id = 10;
    z2->api_id = 11;
    z3->api_id = 12;
    
    
    list <CommandOp *>  p1;
   p1.push_back(z2);
   p1.push_back(z3);
   p1.push_front(z1);
   printf("list size is (%d)\n",p1.size());
   list<CommandOp *>::iterator it1;
   for(it1=p1.begin(); it1!=p1.end(); it1++)
   {
     printf("value of list is (%d) name(%s)\n",(*it1)->api_id,
            ((test *)((*it1)->command_elem))->last);
   }

    list <CommandOp>  *d;
    d = new list <CommandOp>;
    list <CommandOp>  d1;
    list <CommandOp>  d2;
 //   d->push_back(x);
   d1.push_front(x);
   d1.push_back(y);
   d1.push_back(z);
   d1.push_front(y);
   list<CommandOp>::iterator it;
   for(it=d1.begin(); it!=d1.end(); it++)
   {
     printf("value of list is (%d)\n",(*it).api_id);
   }
   //CommandOp n = d1.pop_front();
   //d2.push_back(z);

    map <int, list <CommandOp> *> m_DynamicListMap;

    return 0;
}
