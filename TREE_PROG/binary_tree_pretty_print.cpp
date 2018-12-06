#include <fstream>
#include <iostream>
#include <deque>
#include <iomanip>
#include <sstream>
#include <string>
#include <math.h>
using namespace std;

struct Btree {
  Btree *left, *right;
  int val;
  Btree(int val) : left(NULL), right(NULL), val(val) { }
};

class PrettyPrint
{
public:
    PrettyPrint(){};
    ~PrettyPrint(){};
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
int PrettyPrint::maxHeight(Btree *p) {
  if (!p) return 0;
  int leftHeight = maxHeight(p->left);
  int rightHeight = maxHeight(p->right);
  return (leftHeight > rightHeight) ? leftHeight + 1: rightHeight + 1;
}

// Convert an integer value to string
string PrettyPrint::intToString(int val) {
  ostringstream ss;
  ss << val;
  return ss.str();
}

// Print the arm branches (eg, /    \ ) on a line
void PrettyPrint::printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<Btree*>& nodesQueue, ostream& out) {
  deque<Btree*>::const_iterator iter = nodesQueue.begin();
  for (int i = 0; i < nodesInThisLevel / 2; i++) {  
    out << ((i == 0) ? setw(startLen-1) : setw(nodeSpaceLen-2)) << "" << ((*iter++) ? "/" : " ");
    out << setw(2*branchLen+2) << "" << ((*iter++) ? "\\" : " ");
  }
  out << endl;
}

// Print the branches and node (eg, ___10___ )
void PrettyPrint::printNodes(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<Btree*>& nodesQueue, ostream& out) {
  deque<Btree*>::const_iterator iter = nodesQueue.begin();
  for (int i = 0; i < nodesInThisLevel; i++, iter++) {
    out << ((i == 0) ? setw(startLen) : setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->left) ? setfill('_') : setfill(' '));
    out << setw(branchLen+2) << ((*iter) ? intToString((*iter)->val) : "");
    out << ((*iter && (*iter)->right) ? setfill('_') : setfill(' ')) << setw(branchLen) << "" << setfill(' ');
  }
  out << endl;
}

// Print the leaves only (just for the bottom row)
void PrettyPrint::printLeaves(int indentSpace, int level, int nodesInThisLevel, const deque<Btree*>& nodesQueue, ostream& out) {
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
void PrettyPrint::printPretty(Btree *root, int level, int indentSpace, ostream& out) {
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

#if 0
int main() {
  Btree *root = new Btree(30);
  root->left = new Btree(20);
  root->right = new Btree(40);
  root->left->left = new Btree(10);
  root->left->right = new Btree(25);
  root->right->left = new Btree(35);
  root->right->right = new Btree(50);
  root->left->left->left = new Btree(5);
  root->left->left->right = new Btree(15);
  root->left->right->right = new Btree(28);
  root->right->right->left = new Btree(41);

  PrettyPrint pretty;
  cout << "Tree pretty print with level=1 and indentSpace=0\n\n";
  // Output to console
  pretty.printPretty(root, 1, 0, cout);

  cout << "\n\nTree pretty print with level=5 and indentSpace=3,\noutput to file \"tree_pretty.txt\".\n\n";
  // Create a file and output to that file
  ofstream fout("tree_pretty.txt");
  // Now print a tree that's more spread out to the file
  pretty.printPretty(root, 5, 0, fout);

  return 0;
}
#endif
