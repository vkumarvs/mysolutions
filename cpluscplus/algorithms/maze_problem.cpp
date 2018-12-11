/*
The Problem
A robot is asked to navigate a maze. It is placed at a certain position (the starting position) in the maze and is asked to try to reach another position (the goal position). Positions in the maze will either be open or blocked with an obstacle. Positions are identified by (x,y) coordinates.

[Example of a simple maze] At any given moment, the robot can only move 1 step in one of 4 directions. Valid moves are:
                                                N
    Go North: (x,y) -> (x,y-1)                  |
    Go East: (x,y) -> (x+1,y)                   |
    Go South: (x,y) -> (x,y+1)         W<-------|-------->E
    Go West: (x,y) -> (x-1,y)                   |
                                                S
Note that positions are specified in zero-based coordinates (i.e., 0...size-1, where size is the size of the maze in the corresponding dimension).
The robot can only move to positions without obstacles and must stay within the maze.
The robot should search for a path from the starting position to the goal position (a solution path) until it finds one or until it exhausts all possibilities. In addition, it should mark the path it finds (if any) in the maze. 

Example:
Initial Maze: 
                   -------------------------

                   ------------------------

                   -----------------------
                   
                   -----------------------



Algorithm:
 FIND-PATH(x, y)

    if (x,y outside maze) return false
    if (x,y is goal) return true
    if (x,y not open) return false
    mark x,y as part of solution path
    if (FIND-PATH(North of x,y) == true) return true
    if (FIND-PATH(East of x,y) == true) return true
    if (FIND-PATH(South of x,y) == true) return true
    if (FIND-PATH(West of x,y) == true) return true
    unmark x,y as part of solution path
    return false 

*/

#include <iostream>
using namespace std;

struct MazeCordinates {
    MazeCordinates():east_(false),north_(false),west_(false),south_(false) {}
    MazeCordinates(bool east, bool north, bool west, bool south):
        east_(east),north_(north),west_(west),south_(south) {}
    bool east_;
    bool north_;
    bool west_;
    bool south_;
};

int main() {
  MazeCordinates maze[4][3] = {
                               {MazeCordinates(true, false,false,false), MazeCordinates(false, true,true,false), MazeCordinates(false, false,false,false)}, 
                               {MazeCordinates(true, true, false,false), MazeCordinates(false, false,true,false), MazeCordinates(true, false,false,false)}, 
                               {MazeCordinates(true, false, false, true), MazeCordinates(true, false, true,false), MazeCordinates(false, false,true,false)}, 
                               {MazeCordinates(false, false,false,false), MazeCordinates(false, false,false,false), MazeCordinates(false, false,false,false)}, 
                               };
   cout << "size of maze structure:" << sizeof(MazeCordinates) << endl;
   for (int x=0; x < 4; x++) {
     for (int y=0; y< 3; y++) {
       if(y < 2)
         cout << ".---";
       else
         cout << ".";
     }
     if(x < 3)
     cout << "\n|\n|\n";
     else
       cout <<"\n";
   }
  return 0;
}



