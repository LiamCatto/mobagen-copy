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
      currentPoint = Point2D(k, i);
      currentState = world.Get(currentPoint);

      // Check rules for live cells
      if (currentState == 1) {
        // // Check for underpopulation
        if (CountNeighbors(world, currentPoint) < 2) {
          finalState = 0;
        } // Bug: All previous cells are suddenly set to alive

        // Check if the cell will live on
        if (CountNeighbors(world, currentPoint) == 2 || CountNeighbors(world, currentPoint) == 3) {
          finalState = 1;
        } // Bug: All cells are suddenly set to alive

        // Check for overpopulation
        if (CountNeighbors(world, currentPoint) > 3) {
          finalState = 0;
        } // Bug: All cells are suddenly set to alive
      } else {  // Check rules for dead cells
        // Check for reproduction
        if (CountNeighbors(world, currentPoint) == 2) {
          finalState = 1;
        } // Bug: All cells are suddenly set to alive
      }

      world.SetNext(currentPoint, finalState);
    }
  }
}

int JohnConway::CountNeighbors(World& world, Point2D point) {
  // todo: implement

  int liveNeighbors = 0;
  Point2D topLeftAdjacent (point.x - 1, point.y - 1);   // The cell up and to the left of the point

  for (int i = topLeftAdjacent.y; i < point.y + 2; i++) {
    for (int k = topLeftAdjacent.x; k < point.x + 2; k++) {
      if (world.Get(Point2D(k, i)) == true && Point2D(k, i) != point) {
        liveNeighbors++;
        std::cout << 1;
      }
    }
  }
  std::cout << std::endl;
  std::cout << liveNeighbors << ", " << point.x << ", " << point.y << std::endl;
  return liveNeighbors;
}


// Note to self: step represents a change in world state -- iterate through each cell once.
// buffer holds an entire world state. Will need to set the buffer.