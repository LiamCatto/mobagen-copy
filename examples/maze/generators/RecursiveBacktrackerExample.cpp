#include "../World.h"
#include "Random.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>
bool RecursiveBacktrackerExample::Step(World* w) {
  // todo: implement this

  // General variables
  int currentNeighbor = 0;

  // Random integer list
  int randomList[] = {72, 99, 56, 34, 43, 62, 31, 4, 70, 22, 6, 65, 96, 71, 29, 9, 98, 41, 90, 7, 30, 3, 97, 49, 63, 88, 47, 82, 91, 54, 74, 2, 86, 14, 58, 35, 89, 11, 10, 60, 28, 21, 52, 50, 55, 69, 76, 94, 23, 66, 15, 57, 44, 18, 67, 5, 24, 33, 77, 53, 51, 59, 20, 42, 80, 61, 1, 0, 38, 64, 45, 92, 46, 79, 93, 95, 37, 40, 83, 13, 12, 78, 75, 73, 84, 81, 8, 32, 27, 19, 87, 85, 16, 25, 17, 68, 26, 39, 48, 36};
  int index = 0;

  // Check if the map is empty or completed
  if (stack.empty())
  {
    stack.push_back(randomStartPoint(w));
  } else if (stack.size() == 1 && getVisitables(w, stack.front()).empty()) return false;

  // Get a list of visitable neighbors
  std::vector<Point2D> visitableNeighbors = getVisitables(w, stack.back());

  // Make a decision based on the number of visitable neighbors
  if (visitableNeighbors.size() > 0)  // Select a visitable neighbor to move to
  {
    if (visitableNeighbors.size() > 1)
    {
      currentNeighbor = randomList[index] % visitableNeighbors.size();
      index++;
    }
    else currentNeighbor = 1;
    // todo (Liam): destroy the wall between the two rooms
    stack.push_back(visitableNeighbors[currentNeighbor]);
  } else if (visitableNeighbors.size() == 0)  // Backtrack
  {
    visited[stack.back().x][stack.back().y] = true;
    stack.pop_back();
  }

  return true;  // Buffer overflow (I think?) after this point
}

void RecursiveBacktrackerExample::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize() / 2;

  for (int i = -sideOver2; i <= sideOver2; i++) {
    for (int j = -sideOver2; j <= sideOver2; j++) {
      visited[i][j] = false;
    }
  }
}

Point2D RecursiveBacktrackerExample::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize() / 2;

  // todo: change this if you want
  for (int y = -sideOver2; y <= sideOver2; y++)
    for (int x = -sideOver2; x <= sideOver2; x++)
      if (!visited[y][x]) return {x, y};
  return {INT_MAX, INT_MAX};
}

std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& p) {
  auto sideOver2 = w->GetSize() / 2;
  std::vector<Point2D> visitables;

  Point2D currentPoint;

  // todo: implement this

  // Check the neighbors above and below
  for (int i = p.y - 1; i <= p.y + 1; i += 2)
  {
    currentPoint = Point2D(i, p.x);
    if (!visited[currentPoint.x][currentPoint.y]) visitables.push_back(currentPoint);
  }

  // Check the neighbors right and left
  for (int i = p.x + 1; i <= p.x - 1; i -= 2)
  {
    currentPoint = Point2D(i, p.x);
    if (!visited[currentPoint.x][currentPoint.y]) visitables.push_back(currentPoint);
  }

  // Swap items at index 1 and 2 so the neighbors will be in clockwise order
  currentPoint = visitables[1];
  visitables[1] = visitables[2];
  visitables[2] = currentPoint;

  return visitables;
}
