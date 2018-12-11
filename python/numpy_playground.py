#https://www.machinelearningplus.com/python/numpy-tutorial-python-part2/
import numpy as np
import timeit

#s = np.random.randint(20, size=(3,3))
#s=np.array(y,dtype='bool')
'''
print(s)
y=s.copy()
print (y)
print(id(y),id(s))
print(s.mean(axis=0))
'''
#The key difference between an numpy array and a python list is,
# arrays are designed to handle vectorized operations while a python list is not.
#A numpy array must have all items to be of the same data type, unlike lists. This is another significant difference.
'''
print("arr2d\n",arr2d)
print("arr2d type is:", arr2d.dtype, "\n")
print("s type is:",s.dtype,"\n")
print("s shape is:",s.shape,"\n")
list2 = [[0,1,2], [3,4,5], [6,7,8]]
arr2d = np.array(list2,dtype='float')

arr2d=arr2d.astype('int')
print("recasting as.format{0} arr2d type is:", "int", arr2d.dtype,"\n")
print("arr2d\n", arr2d)


print(type(s))
print("s type is:",s.dtype,"\n")
print("s shape is:",s.shape,"\n")

#s=s.astype('float')
s=s.astype('float')
s[0,2] = np.nan
s[1,2] = np.inf
print(s)

# np.isnan can be applied to NumPy arrays of native dtype (such as np.float64)
missing = np.isnan(s) | np.isinf(s)
print(missing)
s[missing] = -1
print(s)

'''

list2 = [[0,1,2], [3,4,5], [6,7,8]]
arr2d = np.array(list2,dtype='int8')
print(arr2d)
#reverse the array
reverseArr = arr2d[::-1, ::-1]
print(reverseArr)
b = arr2d > 3
print(arr2d[~b])


arr = np.random.randint(1,6,size=(8,4))
print ("Original array\n",arr)
sorted_index_1stcol = arr[:, 0].argsort()
print("Sorted array\n",arr[sorted_index_1stcol])
print ("After sort original array\n", arr)

x = np.arange(10)
print(x)
bins = np.array([0, 3, 6, 9])
print(bins)