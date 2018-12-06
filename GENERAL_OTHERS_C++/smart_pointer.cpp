/*
 * 456 down vote accepted
A smart pointer is a class that wraps a "bare" C++ pointer, to manage the lifetime of the object being pointed to.

With "bare" C++ pointers, the programmer has to explicitly destroy the object when it is no longer useful.

// Need to create the object to achieve some goal
MyObject* ptr = new MyObject(); 
ptr->DoSomething();// Use the object in some way.
delete ptr; // Destroy the object. Done with it.
// Wait, what if DoSomething() raises an exception....

A smart pointer by comparison defines a policy as to when the object is destroyed. You still have to create the object, but you no longer have to worry about destroying it.

SomeSmartPtr<MyObject> ptr(new MyObject());
ptr->DoSomething(); // Use the object in some way.

// Destruction of the object happens, depending 
// on the policy the smart pointer class uses.

// Destruction would happen even if DoSomething() 
// raises an exception

The simplest policy in use involves the scope of the smart pointer wrapper object, such as implemented by boost::scoped_ptr or std::unique_ptr.

void f()
{
    {
       boost::scoped_ptr<MyObject> ptr(new MyObject());
       ptr->DoSomethingUseful();
    } // boost::scopted_ptr goes out of scope -- 
      // the MyObject is automatically destroyed.

    // ptr->Oops(); // Compile error: "ptr" not defined
                    // since it is no longer in scope.
}

Note that scoped_ptr instances cannot be copied. This prevents the pointer from being deleted multiple times (incorrectly). You can however pass references to it around to other functions you call.

Scoped pointers are useful when you want to tie the lifetime of the object to a particular block of code, or if you embedded it as member data inside another object, the lifetime of that other object. The object exists until the containing block of code is exitted, or until the containing object is itself destroyed.

A more complex smart pointer policy involves reference counting the pointer. This does allow the pointer to be copied. When the last "reference" to the object is destroyed, the object is deleted. This policy is implemented by boost::shared_ptr and std::shared_ptr.

void f()
{
    typedef std::tr1::shared_ptr<MyObject> MyObjectPtr; // Nice short alias.
    MyObjectPtr p1; // Empty
    {
        MyObjectPtr p2(new MyObject());
        // There is now one "reference" to the created object
        p1=p2; // Copy the pointer.
        // There are now two references to the object.
    } // p2 is destroyed, leaving one reference to the object.
} // p1 is destroyed, leaving a reference count of zero. 
  // The object is deleted.

Reference counted pointers are very useful when the lifetime of your object is much more complicated, and is not tied directly to a particular section of code or to another object.

There is one drawback to reference counted pointers — the possibility of creating a dangling reference.

// Create the smart pointer on the heap
MyObjectPtr* pp = new MyObjectPtr(new MyObject())
// Hmm, we forgot to destroy the smart pointer,
// because of that, the object is never destroyed!

Another possibility is creating circular references.

struct Owner {
   boost::shared_ptr<Owner> other;
};

boost::shared_ptr<Owner> p1 (new Owner());
boost::shared_ptr<Owner> p2 (new Owner());
p1->other = p2; // p1 references p2
p2->other = p1; // p2 references p1

// Oops, the reference count of of p1 and p2 never goes to zero!
// The objects are never destroyed!

To work around this problem, both boost and C++11 define weak_ptr to define a weak (uncounted) reference to a shared_ptr.

Also note that the existing standard C++ library does define a special kind of smart pointer std::auto_ptr. It is very much like a scoped pointer, except that it also has the "special" dangerous ability to be copied — which also unexpectedly transfers ownership!

std::auto_ptr<MyObject> p1 (new MyObject());
std::auto_ptr<MyObject> p2 = p1; // Copy and transfer ownership. 
                                 // p1 gets set to empty!
p2->DoSomething(); // Works.
p1->DoSomething(); // Oh oh. Hopefully raises some NULL pointer exception.


*/


#include <iostream>
#include <sstream> //


int main()
{
   return 0;
}
