#include <fstream>
#include <iostream>
#include <deque>
#include <iomanip>
#include <sstream>
#include <string>
#include <math.h>
using namespace std;
#if 0
struct Btree {
  Btree *left, *right;
  int val;
  Btree(int val) : left(NULL), right(NULL), val(val) { }
};
#endif
struct Btree{

  Btree()
  {
    val = INT_MAX;
    id = INT_MAX;
    left = right = parent = NULL;
  }

  Btree(int _val, int _id = 0)
  {
    id = _id;
    val = _val;
    parent = NULL;
    left = NULL;
    right = NULL;
  }

  int id, val;
  Btree *left, *right, *parent;
};

class PrettyBinaryPrint
{
public:
    PrettyBinaryPrint(){};
    ~PrettyBinaryPrint(){};
     int maxHeight(Btree *p);
     string intToString(int val); 
void printBranches(int branchLen, int nodeSpaceLen, int startLen, int
                   nodesInThisLevel, const deque<Btree*>& nodesQueue, ostream&
                   out); 
void printNodes(int branchLen, int nodeSpaceLen, int startLen, int
                nodesInThisLevel, const deque<Btree*>& nodesQueue, ostream&
                out); 
void printLeaves(int indentSpace, int level, int nodesInThisLevel, const
                 deque<Btree*>& nodesQueue, ostream& out);
void printPretty(Btree *root, int level, int indentSpace, ostream& out);
};

// Find the maximum height of the binary tree
int PrettyBinaryPrint::maxHeight(Btree *p) {
  if (!p) return 0;
  int leftHeight = maxHeight(p->left);
  int rightHeight = maxHeight(p->right);
  return (leftHeight > rightHeight) ? leftHeight + 1: rightHeight + 1;
}

// Convert an integer value to string
string PrettyBinaryPrint::intToString(int val) {
  ostringstream ss;
  ss << val;
  return ss.str();
}

// Print the arm branches (eg, /    \ ) on a line
void PrettyBinaryPrint::printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<Btree*>& nodesQueue, ostream& out) {
  deque<Btree*>::const_iterator iter = nodesQueue.begin();
  for (int i = 0; i < nodesInThisLevel / 2; i++) {  
    out << ((i == 0) ? setw(startLen-1) : setw(nodeSpaceLen-2)) << "" << ((*iter++) ? "/" : " ");
    out << setw(2*branchLen+2) << "" << ((*iter++) ? "\\" : " ");
  }
  out << endl;
}

// Print the branches and node (eg, ___10___ )
void PrettyBinaryPrint::printNodes(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<Btree*>& nodesQueue, ostream& out) {
  deque<Btree*>::const_iterator iter = nodesQueue.begin();
  for (int i = 0; i < nodesInThisLevel; i++, iter++) {
    out << ((i == 0) ? setw(startLen) : setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->left) ? setfill('_') : setfill(' '));
    out << setw(branchLen+2) << ((*iter) ? intToString((*iter)->val) : "");
    out << ((*iter && (*iter)->right) ? setfill('_') : setfill(' ')) << setw(branchLen) << "" << setfill(' ');
  }
  out << endl;
}

// Print the leaves only (just for the bottom row)
void PrettyBinaryPrint::printLeaves(int indentSpace, int level, int nodesInThisLevel, const deque<Btree*>& nodesQueue, ostream& out) {
  deque<Btree*>::const_iterator iter = nodesQueue.begin();
  for (int i = 0; i < nodesInThisLevel; i++, iter++) {
    out << ((i == 0) ? setw(indentSpace+2) : setw(2*level+2)) << ((*iter) ? intToString((*iter)->val) : "");
  }
  out << endl;
}

// Pretty formatting of a binary tree to the output stream
// @ param
// level  Control how wide you want the tree to sparse (eg, level 1 has the minimum space between nodes, while level 2 has a larger space between nodes)
// indentSpace  Change this to add some indent space to the left (eg, indentSpace of 0 means the lowest level of the left node will stick to the left margin)
void PrettyBinaryPrint::printPretty(Btree *root, int level, int indentSpace, ostream& out) {
  int h = maxHeight(root);
  int nodesInThisLevel = 1;

  int branchLen = 2*((int)pow(2.0,h)-1) - (3-level)*(int)pow(2.0,h-1);  // eq of the length of branch for each node of each level
  int nodeSpaceLen = 2 + (level+1)*(int)pow(2.0,h);  // distance between left neighbor node's right arm and right neighbor node's left arm
  int startLen = branchLen + (3-level) + indentSpace;  // starting space to the first node to print of each level (for the left most node of each level only)
    
  deque<Btree*> nodesQueue;
  nodesQueue.push_back(root);
  for (int r = 1; r < h; r++) {
    printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
    branchLen = branchLen/2 - 1;
    nodeSpaceLen = nodeSpaceLen/2 + 1;
    startLen = branchLen + (3-level) + indentSpace;
    printNodes(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);

    for (int i = 0; i < nodesInThisLevel; i++) {
      Btree *currNode = nodesQueue.front();
      nodesQueue.pop_front();
      if (currNode) {
	      nodesQueue.push_back(currNode->left);
	      nodesQueue.push_back(currNode->right);
      } else {
        nodesQueue.push_back(NULL);
        nodesQueue.push_back(NULL);
      }
    }
    nodesInThisLevel *= 2;
  }
  printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue, out);
  printLeaves(indentSpace, level, nodesInThisLevel, nodesQueue, out);
}
