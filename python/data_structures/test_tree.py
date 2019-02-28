import pytest

class MyTreeNode:
    def __init__(self):
        self.value = None
        self.lChild = None
        self.rChild = None

class MyBST:
    def __init__(self):
        self.node = None

    def addNode(self, value):
        if self.node == None:
            self.node = MyTreeNode()
            self.node.value = value
            self.node.lChild = None
            self.node.rChild = None
            return

        temp = self.node
        while(temp != None):
            if(value >= temp.value):
                if temp.rChild == None:
                    print("right side")
                    temp.rChild = MyTreeNode()
                    temp.rChild.value = value
                    temp.rChild.rChild = None
                    temp.rChild.lChild = None
                    break;
                else:
                    temp = temp.rChild
            elif(value <= temp.value):
                if temp.lChild == None:
                    print("left side")
                    temp.lChild = MyTreeNode()
                    temp.lChild.value = value
                    temp.lChild.rChild = None
                    temp.lChild.lChild = None
                    break;
                else:
                    temp = temp.lChild

    def getRoot(self):
        return self.node

    def values(self, node, list1):
        if node == None:
            return list1
        self.values(node.lChild, list1)
        list1.append(node.value)
        self.values(node.rChild,list1)

'''
myBst = MyBST()
myBst.addNode(10)
myBst.addNode(8)
myBst.addNode(30)
myBst.addNode(7)
myBst.addNode(9)
myBst.addNode(20)
myBst.addNode(35)
list1=[]
myBst.values(myBst.getRoot(), list1)
for i in list1:
    print(i)
'''

class TestMyBstTree:
    def test_empty_tree(self):
        mybst = MyBST()
        assert mybst.getRoot() == None

    def test_add_node(self):
        myBst = MyBST()
        myBst.addNode(10)
        myBst.addNode(8)
        myBst.addNode(30)
        myBst.addNode(7)
        myBst.addNode(9)
        myBst.addNode(20)
        myBst.addNode(35)
        list1=[]
        myBst.values(myBst.getRoot(), list1)
        assert list1 == [7,8,9,10,20,30,35]

pytest.main()


