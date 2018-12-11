import timeit
'''
setup = '''
import random

random.seed('slartibartfast')
s = [random.random() for i in range(1000)]
timsort = list.sort
'''
print (min(timeit.Timer('a=s[:]; timsort(a)', setup=setup).repeat(7, 1000)))
'''

import numpy as np

np.random.seed(1)
s = np.random.randint(2,100, 20)

print (s)
aCopy = s.copy()   #aCopy = s[:] #Will create a copy of the s
a = s #Will refer to the same object as it is an assignment
print(id(s))
print(id(a))
print(id(aCopy))
print("After sorting\n");
aCopy.sort()

#Why below statement showing the same sorted array when
print(a)
print (s)
print (aCopy)

print(id(s))
print(id(a))
print(id(aCopy))


#Normal list copy doesn't modify the original array
'''
#Second way
import random
random.seed('slartibartfast')
s = [random.random() for i in range(4)]
print(s)
timesort = list.sort
a=s[:];
timesort(a)
print(a)
'''

'''
c=[1,2,3,4,5,6]
d=c[:]

print(id(c))
print(id(d))
'''



'''
a = 10;
list1 = [1,2,3]
print(id(a));
def function1(a,list1):
    #a=20
    #list1.append(100)
    list1[0]=20
    print(list1)
    print(id(a));

print (a)
print("list is:\n")
print(list1)
print("after function call\n")
print(function1(a,list1))
print("new list is:\n")
print(list1)
'''

'''
Given one of the answers here is very misleading, 
I’ll attempt to provide a more correct explanation.
All variables are basically references to objects. 
They are labels we use to refer to any building block in 
Python (an int, a string, a list, a tuple, a class, an object, 
a function, etc.). The references, however, are immutable - the only allowed operation is assignment:

In [1]: a = 1
 
In [2]: id(a)
Out[2]: 10055552
 
In [3]: a = 2
 
In [4]: id(a)
Out[4]: 10055584
 
In [5]: a = [1, 2, 3]
 
In [6]: id(a)
Out[6]: 140620817520328
The variables cannot point to other variables, they always point to the underlying object instead:

In [7]: a = 1
 
In [8]: id(a)
Out[8]: 10055552
 
In [9]: b = a
 
In [10]: id(b)
Out[10]: 10055552
 
In [11]: a = 2
 
In [12]: a
Out[12]: 2
 
In [13]: b
Out[13]: 1
 
In [14]: id(a)
Out[14]: 10055584
 
In [15]: id(b)
Out[15]: 10055552
 
In [16]: a = [1, 2]
 
In [17]: id(a)
Out[17]: 140620834911048
 
In [18]: b = a
 
In [19]: id(b)
Out[19]: 140620834911048
 
In [20]: a = [2, 3]
 
In [21]: id(a)
Out[21]: 140620834901512
 
In [22]: id(b)
Out[22]: 140620834911048
As you can see, after assigning a and b to the same object,
 the memory address to which b points stays the same, even after we assign something else to a.
On the other hand, modifying a mutable type won’t create a new object and change the references:

In [23]: a = [1, 2]
 
In [24]: b = a
 
In [25]: b
Out[25]: [1, 2]
 
In [26]: a.append(3)
 
In [27]: id(a)
Out[27]: 140620834901768
 
In [28]: id(b)
Out[28]: 140620834901768
That’s important to remember when passing variables as arguments: 
if you want to modify them, you need to mutate the object itself, 
you can’t create a new object and assign it to the same identifier, 
as it would just create a new object in new memory address, 
and the original reference would still be pointing to the old object. 
In that context, variables in Python do not act as a C-style pointers, where you can modify memory it points to freely.

In [49]: def foo_int(a):
    ...:     a = 2
    ...:
 
In [50]: a = 1
 
In [51]: foo_int(a)
 
In [52]: a
Out[52]: 1
 
In [53]: def foo_list(a):
    ...:     a = [2]
    ...:
 
In [54]: a = [1]
 
In [55]: foo_list(a)
 
In [56]: a
Out[56]: [1]
 
In [57]: def foo_list_mutable(a):
    ...:     a.append(2)
    ...:
 
In [58]: a = [1]
 
In [59]: foo_list_mutable(a)
 
In [60]: a
Out[60]: [1, 2]
If, on the other hand, you want to have a copy of a mutable object, 
instead of reference to it, you can use the copy module 
(8.10. copy - Shallow and deep copy operations - Python 3.6.3 documentation):

In [94]: import copy
 
In [95]: a = [1]
 
In [96]: foo_list_mutable(copy.copy(a))
 
In [97]: a
Out[97]: [1]
There are, however, tools in Python that allow for creation of pointers 
and interfacing with C, if you’re interested: 16.16. ctypes - 
A foreign function library for Python - Python 3.6.3 documentation.
'''