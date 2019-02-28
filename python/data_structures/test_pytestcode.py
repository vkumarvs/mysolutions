import pytest

'''
http://blog.chapagain.com.np/hash-table-implementation-in-python-data-structures-algorithms/ --
convert this code into ur own implementation
'''
class MySet:
    def __init__(self):
        self.lSet = set()

    def insert(self, value):
        self.lSet.add(value)
        return self.lSet

    def values(self):
        return self.lSet

    def count(self):
        return len(self.lSet)

class TestMySet:

    def test_empty(self):
        my_set = MySet()
        assert my_set.count() == 0

    def test_case_1(self):
        my_set = MySet()

        assert 2 in my_set.insert(2)

    def test_add_and_read_set(self):
        my_set = MySet()
        value = 1
        my_set.insert(value)

        assert my_set.values() == {value}, 'expected only the item I have added'

    def test_only_add_one_per_value(self):
        my_set = MySet()

        my_set.insert(1)
        my_set.insert(2)
        my_set.insert(1)
        my_set.insert(2)

        assert my_set.count() == 2
        assert my_set.values() == {1, 2}, 'expected the two items I have seen'

pytest.main()
