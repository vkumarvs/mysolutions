#include <iostream>
using namespace std;


struct Block {
	Block() {isOpen = true; isVisited = false;}
	bool isOpen;
	bool isVisited;
	friend ostream& operator<<(ostream& os, const Block &x);
};

ostream& operator<<(ostream& os, const Block& x) {
	//os <<"isOpen:" <<(x.isOpen?"True":"False");
	os <<x.isOpen ;
    return os;
}

void displayMaze(Block _maze[][4]) {
	for(int x = 0; x < 4; x++) {
	    cout <<endl;
		for (int y = 0; y < 4; y++) {
			cout << _maze[x][y] <<"\t";
		}
	}
	cout <<endl;
}

void displayPath(Block _maze[][4]) {
    cout <<"New Path:"<<endl;
	for(int x = 0; x < 4; x++) {
		cout <<endl;
		for (int y = 0; y < 4; y++) {
			if(_maze[x][y].isVisited) {
				cout <<"p" <<"\t";
			} else {
				cout <<"#" <<"\t";
			}
		}
		cout <<endl;
	}
}

void defineMaze1(Block _maze[][4]) {
   _maze[0][2].isOpen = false;
   _maze[2][1].isOpen = false;
   _maze[3][1].isOpen = false;
   _maze[3][2].isOpen = false;
   displayMaze(_maze);
}


void findPath(Block _maze[][4], int row, int col) {
     if(row >= 4 || col >= 4) return;

	if(row == 3 && col == 3) {
		displayPath(_maze);
		return;
	}

	if(!_maze[row][col].isOpen) {
		return;
	}
	_maze[row][col].isVisited = true;

	if(_maze[row][col + 1].isOpen) {
		findPath(_maze, row, col+1);
	}

	if(_maze[row + 1][col].isOpen) {
		findPath(_maze, row+1, col);
	}
	_maze[row][col].isVisited = false; //reset the visit flag during backtraking
}

int main(){
    Block _maze[4][4];
     defineMaze1(_maze);
     findPath(_maze, 0, 0);
	return 0;
}
