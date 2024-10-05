#include "../World.h"
#include "Random.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>
#include <string>
bool RecursiveBacktrackerExample::Step(World* w) {
  // todo: implement this

  // General variables
  int currentNeighbor = 0;
  Vector2f neighborDirection;

  // Random integer list and other random values
  static int randomList[] = {72, 99, 56, 34, 43, 62, 31, 4, 70, 22, 6, 65, 96, 71, 29, 9, 98, 41, 90, 7, 30, 3, 97, 49, 63, 88, 47, 82, 91, 54, 74, 2, 86, 14, 58, 35, 89, 11, 10, 60, 28, 21, 52, 50, 55, 69, 76, 94, 23, 66, 15, 57, 44, 18, 67, 5, 24, 33, 77, 53, 51, 59, 20, 42, 80, 61, 1, 0, 38, 64, 45, 92, 46, 79, 93, 95, 37, 40, 83, 13, 12, 78, 75, 73, 84, 81, 8, 32, 27, 19, 87, 85, 16, 25, 17, 68, 26, 39, 48, 36};
  static int index = 0;
  if (index > 100) index = 0;
  Point2D randP = randomStartPoint(w);

  // Bootstrap case (written by professor)
  if (stack.empty() && !visited[randP.x][randP.y]) {
    stack.emplace_back(randomStartPoint(w));
    w->SetNodeColor(randP, Color::Orange);
    visited[randP.x][randP.y] = true;
    return true;
  } else if (stack.size() == 1 && getVisitables(w, stack.front()).empty())
  {
    w->SetNodeColor(stack.back(), Color::Black);
    Clear(w);
    return false;
  }

  // // Check if the map is empty or completed
  // if (stack.empty())
  // {
  //   stack.emplace_back(randomStartPoint(w));
  // } else if (stack.size() == 1 && getVisitables(w, stack.front()).empty()) return false;

  // Get a list of visitable neighbors
  std::vector<Point2D> visitableNeighbors = getVisitables(w, stack.back());

  // Select a node to move to based on the number of visitable neighbors
  if (visitableNeighbors.size() > 0)  // Select a visitable neighbor to move to
  {
    // Pick a neighbor to visit
    if (visitableNeighbors.size() > 1)
    {
      currentNeighbor = randomList[index] % visitableNeighbors.size();
      index++;
    }
    else currentNeighbor = 0;

    // todo (Liam): destroy the wall between the two rooms
    neighborDirection = Vector2f(visitableNeighbors[currentNeighbor].x - stack.back().x, visitableNeighbors[currentNeighbor].y - stack.back().y);
    std::cout << neighborDirection.x << ", " << neighborDirection.y << std::endl;

    if (neighborDirection == Vector2f(0, -1))
    {
      w->SetNorth(stack.back(), false);
    } else if (neighborDirection == Vector2f(1, 0))
    {
      w->SetEast(stack.back(), false);
    } else if (neighborDirection == Vector2f(0, 1))
    {
      w->SetSouth(stack.back(), false);
    } else if (neighborDirection == Vector2f(-1, 0))
    {
      w->SetWest(stack.back(), false);
    }

    // Move to the next node
    stack.emplace_back(visitableNeighbors[currentNeighbor]);
    w->SetNodeColor(stack.back(), Color::Orange);
    visited[stack.back().x][stack.back().y] = true;
  } else if (visitableNeighbors.size() == 0)  // Backtrack
  {
    w->SetNodeColor(stack.back(), Color::Black);
    stack.pop_back();
  }

  return true;
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

  Point2D const neighbor[] = {Point2D(p.x, p.y - 1), Point2D(p.x + 1, p.y), Point2D(p.x, p.y + 1), Point2D(p.x - 1, p.y)};
  bool isInStack = false;

  // todo: implement this

  // Check the neighbors above and below
  for (int i = 0; i < 4; i++)
  {
    // Check if the point is in the stack or was visited
    for (int k = 0; k < stack.size(); k++)
    {
      if (stack[k] == neighbor[i])
      {
        isInStack = true;
        break;
      } else isInStack = false;
    }

    // Check if the point is within the world borders (y is less than top bound / y is greater than bottom bound / x is less than top bound / x is greater than top bound)
    if (neighbor[i].y >= -1 * sideOver2 && neighbor[i].y <= sideOver2 && neighbor[i].x >= -1 * sideOver2 && neighbor[i].x <= sideOver2)
    {
      if (!visited[neighbor[i].x][neighbor[i].y] && !isInStack) visitables.push_back(neighbor[i]);
    }

  }

  return visitables;
}
