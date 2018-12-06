#include <iostream>
#include <stdlib.h>
#include <string>
#include <climits>
#include <fstream>
#include <queue>
#include <stack>
#include "PrettyBinaryPrint.hpp"

#define USE_QUEUE 1
#define USE_STACK 0

using namespace std;

#if 0
struct Btree
{
    Btree()
    {
      val = INT_MAX;
      id = INT_MAX;
      left = right = parent = NULL;
    }

    Btree(int _val, int _myId)
    {
      val = _val;
      id = _myId;
      left = right = parent = NULL;
    }
    int val;
    int id;
    Btree *left, *right, *parent;
};
#endif

struct DoubleLinkedList
{
    DoubleLinkedList()
    {
      val = NULL;
      next = previous = NULL;
    }
    DoubleLinkedList *next, *previous;
    Btree *val;
};
bool insertElem(Btree &head, int val, int id);
bool writeDataIntoFile(ofstream &file, Btree *node)
{
    if(!node)
      return false;

    file<<node->val;
    file<<" ";
    //file<<node->id;
    if(!node->left)
    {
      file<<INT_MAX;
    file<<" ";
      //file<<'A';
    }

    if(!node->right)
    {
      file<<INT_MIN;
    file<<" ";
      //file<<'B';
    }

      //file<<"---------";

    writeDataIntoFile(file, node->left);
    writeDataIntoFile(file, node->right);
}

bool deSerializeTreeFromFile(Btree &node)
{
    ifstream file("serialize.txt", ifstream::in|ifstream::out);
    if(!file.is_open())
    {
      cout<<"file is not open"<<endl;
      return false;
    }
    int x;
    int elemntId = 1;
    while(file >> x)
    {
      if(node.val == INT_MAX)
      {
            insertElem(node, x, elemntId++);
      }
      else if(x == INT_MAX || x == INT_MIN)
      {
      }
      else
      {
            insertElem(node, x, elemntId++);
      }
    }
    cout<<"finish with file reading"<<endl;

    file.close();
}

bool serializeBtreeToFile(Btree &head)
{
    ofstream file;
    file.open("serialize.txt", ifstream::in|ifstream::out);
    if(!file.is_open())
    {
      cout<<"file is not open"<<endl;
      return false;
    }
    writeDataIntoFile(file, &head);
    file.close();
}

void printPostOrder(Btree *head)
{
    if(!head)
      return;
    printPostOrder(head->left);
    printPostOrder(head->right);
    cout<<head->val <<"\t";
}

bool myParent(Btree *head, int _myId, Btree **parent)
{
    if(!head)
    {
      return false;
    }

    if(_myId/2 == head->id)
    {
      *parent = head;
      return true;
    }

    bool value = myParent(head->left, _myId, parent);
    if(!value)
    return myParent(head->right, _myId, parent);
    else
      return value;
}


void InsertDoubleLinkElem(DoubleLinkedList **head, Btree *val)
{
    if(!val)
    {
      cout<<"new node is null"<<endl;
    }

    DoubleLinkedList *previous = NULL;
    if(!(*head))
    {
      *head = new DoubleLinkedList();
      (*head)->val  = val;
    }
    else
    {
      previous = *head;
      DoubleLinkedList *tmp = (*head)->next;
      while(tmp)
      {
        previous = tmp;
        tmp = tmp->next;
      }
      previous->next = new DoubleLinkedList();
      previous->next->val = val;
      previous->next->previous = previous;
    }
}

void printBFSOrder(Btree *node)
{
    queue<Btree *> myqueue;
    myqueue.push(node);
    while(!myqueue.empty())
    {
      Btree *tempNode = NULL;
      tempNode = myqueue.front();
      cout<<tempNode->val<<"\t";
      myqueue.pop();
      if(tempNode->left)
        myqueue.push(tempNode->left);
      if(tempNode->right)
        myqueue.push(tempNode->right);
    }
}
void printDoublyLinkeOfTree(Btree *node)
{
    DoubleLinkedList *head = NULL;
    queue<Btree *> myqueue;
    stack<Btree *> myStack;
    myqueue.push(node);
    int x = USE_QUEUE;
    while(!myStack.empty() || !myqueue.empty())
    {
      Btree *tempNode = NULL;
      if(x == USE_QUEUE)
      {
        while(!myqueue.empty())
        {
          tempNode = myqueue.front();
          myqueue.pop();
          /*Insert Element in doubleLinked list*/
          InsertDoubleLinkElem(&head, tempNode);
          if(tempNode->left)
            myStack.push(tempNode->left);
          if(tempNode->right)
            myStack.push(tempNode->right);
        }
        x = USE_STACK;
      }
      else if(x == USE_STACK)
      {
        while(!myStack.empty())
        {
          tempNode = myStack.top();
          /*Insert Element in doubleLinked list*/
          InsertDoubleLinkElem(&head, tempNode);
          myStack.pop();
          if(tempNode->right)
            myqueue.push(tempNode->right);
          if(tempNode->left)
            myqueue.push(tempNode->left);
        }
        x = USE_QUEUE;
      }
    }

    /*Print element in double linked list */
    cout<<"Going to print double linked list"<<endl;
    DoubleLinkedList *last = head;
    while(head)
    {
      last = head;
      cout<<head->val->val<<"\t";
      head = head->next;
    }
    cout<<endl;
    cout<<"Going to print reverse order of double linked list"<<endl;
    while(last)
    {
      cout<<last->val->val<<"\t";
      last  = last->previous;
    }
}

void printBFSUsingStack(Btree *node)
{

    stack<Btree *> myqueue;
    myqueue.push(node);
    while(!myqueue.empty())
    {
      Btree *tempNode = NULL;
      tempNode = myqueue.top();
      cout<<tempNode->val<<"\t";
      myqueue.pop();
      if(tempNode->left)
        myqueue.push(tempNode->left);
      if(tempNode->right)
        myqueue.push(tempNode->right);
    }
}

void printZigZagOrder(Btree *node)
{
    queue<Btree *> myqueue;
    stack<Btree *> myStack;

    myqueue.push(node);
    int x = USE_QUEUE;
    while(!myStack.empty() || !myqueue.empty())
    {
      Btree *tempNode = NULL;
      if(x == USE_QUEUE)
      {
        while(!myqueue.empty())
        {
          tempNode = myqueue.front();
          cout<<tempNode->val<<"\t";
          myqueue.pop();
          if(tempNode->left)
            myStack.push(tempNode->left);
          if(tempNode->right)
            myStack.push(tempNode->right);
        }
        x = USE_STACK;
      }
      else if(x == USE_STACK)
      {
        while(!myStack.empty())
        {
          tempNode = myStack.top();
          cout<<tempNode->val<<"\t";
          myStack.pop();
          if(tempNode->right)
            myqueue.push(tempNode->right);
          if(tempNode->left)
            myqueue.push(tempNode->left);
        }
        x = USE_QUEUE;
      }
    }
}

bool insertElem(Btree &head, int val, int id)
{
    if(id == 1)
    {
      head.val = val;
      head.id = id;
      head.left = NULL;
      head.right = NULL; 
      head.parent = NULL;
    }
    else
    {
     // cout<<"value of head is --- "<<head.val <<"id is ----"<<head.id <<endl;
      Btree *parent = NULL;
      myParent(&head, id, &parent);
      if(parent == NULL)
      {
        cout<<"insert element failed"<<endl;
        return false;
      }

      Btree *newNode = new Btree(val, id);
      if(0 == id%2)
      {
        parent->left = newNode;
      }
      else
      {
        parent->right = newNode;
      }
    }
    return true;
}


int main()
{
    Btree head;
    int elemntId = 1;
    char ch;
    cout<<"Enter Options: a: Create from hard coded array\n p:print tree \n s: serialize to file \n d: deserialize into btree"<<endl;

    while(1)
    {
    cout<<"enter option value"<<endl;
    cin>>ch;
      switch(ch)
      {
        case 'a':
        {
          int arr[] = {5,10,30,20,15,19,28,24,26};
          cout<<"size of array --"<<(sizeof(arr)/sizeof(int))<<endl;
          for(int i = 0; i < (sizeof(arr)/sizeof(int)); i++)
          {
            insertElem(head, arr[i], elemntId++);
          }
        }
        break;
        case 'p':
        {
          cout << "Tree pretty print with level=1 and indentSpace=0\n\n";
          PrettyBinaryPrint pretty;
          pretty.printPretty(&head, 1, 0, cout);

          cout<<"PostOrder traversal is "<<endl;
          printPostOrder(&head);
          cout<<endl;
        }
        break;
        case 'l':
        {
          cout<<"Level Order traversal is using queue"<<endl;
          printBFSOrder(&head);
          cout<<endl;
          cout<<"Using stack "<<endl;
          printBFSUsingStack(&head);
          cout<<endl;
          cout<<"Using ZigZag"<<endl;
          printZigZagOrder(&head);
          cout<<endl;
          cout<<"Using Double linked list"<<endl;
          printDoublyLinkeOfTree(&head);
          cout<<endl;
        }
        break;
        case 's':
        {
          bool ret = serializeBtreeToFile(head);
          cout<<"serialization is --- "<< (ret?"true":"false")<<endl;
        }
        break;
        case 'd':
        {
          Btree root;
          bool ret = deSerializeTreeFromFile(root);
          cout<<"After deSerialization tree PostOrder is" <<endl;
          printPostOrder(&root);
          cout << "\nTree pretty print with level=1 and indentSpace=0\n\n";
          PrettyBinaryPrint pretty;
          pretty.printPretty(&head, 1, 0, cout);
          cout<<endl;
        }
        break;
        default:
        {
          cout<<"invalid option"<<endl;
          exit(1);
        }
      }

    }
    return 0;
}
