import pytest

class MyStack:
    def __init__(self):
        self.stack = []

    def push(self, value):
        self.stack.append(value)

    def peek(self):
        return self.stack[len(self.stack)-1]

    def pop(self):
        return self.stack.pop()

    def size(self):
        return (len(self.stack))


class TestMyStack:
    def test_empty_stack(self):
        myStack = MyStack()
        assert myStack.size() == 0

    def test_push(self):
        myStack = MyStack()
        myStack.push(10)
        myStack.push(20)
        myStack.push(30)
        myStack.push(40)
        assert myStack.size() == 4
        assert myStack.peek() == 40

    def test_pop(self):
        myStack = MyStack()
        myStack.push(10)
        myStack.push(20)
        myStack.push(30)
        myStack.push(40)
        assert myStack.size() == 4
        assert myStack.peek() == 40
        assert myStack.pop() == 40
        assert myStack.peek() == 30


pytest.main()
