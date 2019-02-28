import pytest

class MySet:
    def __init__(self):
        self.set = []

    def add(self, value):
        if value not in self.set:
            self.set.append(value)

    def remove(self, value):
        if value in self.test:
            self.set.remove(value)

    def values(self):
        return self.set

    def union(self, s1):
        for i in s1.set:
            if i not in self.set:
                self.set.append(i)

    def intersection(self, s1):
        result=[]
        for i in s1.set:
            if i in self.set:
                result.append(i)
        return result

    def __add__(self, s2): #add operator overloading
        self.union(s2)


class TestMySet:
    def test_empty(self):
        mytest = MySet()
        assert mytest.values() == []

    def test_insert_values(self):
        mytest = MySet()
        for i in range(1,10):
            mytest.add(i)
        assert mytest.values() == [1,2,3,4,5,6,7,8,9], "expected values mismatch"

    def test_insert_duplicates(self):
        mytest = MySet()
        mytest.add(1)
        mytest.add(1)
        mytest.add(2)
        assert mytest.values() == [1,2], "expected values mismatch"

    def test_union(self):
        mytest1 = MySet()
        mytest1.add(1)
        mytest1.add(1)
        mytest1.add(2)
        assert mytest1.values() == [1,2], "expected values mismatch"
        mytest2 = MySet()
        mytest2.add(1)
        mytest2.add(3)
        mytest2.add(4)
        mytest1 + mytest2
        assert mytest1.values() == [1,2,3,4], "expected values mismatch"

    def test_intersection(self):
        mytest1 = MySet()
        mytest1.add(1)
        mytest1.add(1)
        mytest1.add(2)
        assert mytest1.values() == [1,2], "expected values mismatch"
        mytest2 = MySet()
        mytest2.add(1)
        mytest2.add(3)
        mytest2.add(4)
        assert mytest1.intersection(mytest2) == [1], "expected values mismatch"
pytest.main()
