#Linklist in python
class LinkNode:
    def __init__(self):
        self.value = None
        self.nextNode = None
        print("LinkNode is being created:", id(self))
    def __del__(self):
        print("LinkNode is being destroyed:", id(self))

def addNode(head, value=None ):
    if value is None:
        print("Please enter a valid value")
        return
    if head.value is None:
        head.value = value
        head.nextNode=None
        return

    temp = head
    while temp.nextNode is not None:
        print("I will travel")
        temp = temp.nextNode
    #create a new Node with value
    print("Create new node")
    newNode = LinkNode()
    newNode.value = value
    temp.nextNode = newNode
'''
#Below function has an error
def addNode(head, value=None ):
    if value is None:
        print("Please enter a valid value")
        return
    if head is None:
        head = LinkNode()
        head.value = value
        head.nextNode=None
        return

    temp = head
    while temp.nextNode is not None:
        print("I will travel")
        temp = temp.nextNode
    #create a new Node with value
    print("Create new node")
    newNode = LinkNode()
    newNode.value = value
    temp.nextNode = newNode
'''

def displayList(head=None):
    if head is None:
        print("Empty link list\n")
        return
    s=[]
    while head is not None:
        s.append(head.value)
        head = head.nextNode
    print(s,"\n")

def linkListfunction():
    help='\t"a ---- add linklist node\n\
\t d ---- delete a node\n\
\t t ---- display the nodes\n\
\t e ---- exit the program\n'

    print("Actions:\n", help)
    headNode = LinkNode()
    while(True):
        action=input("Please enter action value:\n")
        if action is 'a':
            val = input("Enter Node value:\n")
            addNode(headNode, val)
            displayList(headNode)
        elif action is 'd':
            print("Delete a node from the linklist\n")
        elif action is 't':
            displayList(headNode)
        elif action is 'e':
            print("Actions:\n", help)
            exit()
        else:
            print("Actions:\n", help)
            exit()

def treefunction():
    help='\t"a ---- add linklist node\n\
\t d ---- delete a node\n\
\t t ---- display the nodes\n\
\t e ---- exit the program\n'

    print("Actions:\n", help)
    headNode = LinkNode()
    while(True):
        action=input("Please enter action value:\n")
        if action is 'a':
            val = input("Enter Node value:\n")
            addNode(headNode, val)
            displayList(headNode)
        elif action is 'd':
            print("Delete a node from the linklist\n")
        elif action is 't':
            displayList(headNode)
        elif action is 'e':
            print("Actions:\n", help)
            exit()
        else:
            print("Actions:\n", help)
            exit()

#Start your program
linkListfunction()





