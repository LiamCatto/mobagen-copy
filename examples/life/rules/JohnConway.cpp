#include "JohnConway.h"
#include <iostream>

// Reference: https://playgameoflife.com/info
void JohnConway::Step(World& world) {
  // todo: implement
  Point2D currentPoint(0, 0);
  bool currentState = true;   // The state of the current point
  bool finalState  = true;    // The final state of the current point

  for (int i = 0; i < world.SideSize(); i++) {
    for (int k = 0; k < world.SideSize(); k++) {
      currentPoint = Point2D(i, k);
      currentState = world.Get(currentPoint);

      // Check rules for live cells
      if (currentState == 1) {
        // Check for underpopulation
        if (CountNeighbors(world, currentPoint) < 2) {
          finalState = 0;
        }

        // Check if the cell will live on
        if (CountNeighbors(world, currentPoint) == 2 || CountNeighbors(world, currentPoint) == 3) {
          finalState = 1;
        }

        // Check for overpopulation
        if (CountNeighbors(world, currentPoint) > 3) {
          finalState = 0;
        }
      } else {  // Check rules for dead cells
        // Check for reproduction
        if (CountNeighbors(world, currentPoint) == 2) {
          finalState = 1;
        }
      }

      world.SetNext(currentPoint, finalState);
    }
  }

  world.SwapBuffers();
}

int JohnConway::CountNeighbors(World& world, Point2D point) {
  // todo: implement

  int liveNeighbors = 0;
  Point2D topLeftAdjacent (point.x - 1, point.y - 1);   // The cell up and to the left of the point

  for (int i = topLeftAdjacent.y; i < point.y + 1; i++) {
    for (int k = topLeftAdjacent.x; k < point.x + 1; k++) {
      if (world.Get(Point2D(k, i)) == true && k != point.x && i != point.y) {
        liveNeighbors++;
      }
    }
  }
  //std::cout << liveNeighbors << std::endl;
  return liveNeighbors;
}


// Note to self: step represents a change in world state -- iterate through each cell once.
// buffer holds an entire world state. Will need to set the buffer.