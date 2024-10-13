#include "Agent.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "World.h"
using namespace std;
std::vector<Point2D> Agent::generatePath(World* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  queue<Point2D> frontier;                   // to store next ones to visit
  unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results

  // bootstrap state
  auto catPos = w->getCat();
  frontier.push(catPos);
  frontierSet.insert(catPos);
  Point2D borderExit = Point2D::INFINITE;  // if at the end of the loop we dont find a border, we have to return random points

  std::cout << "catPos: " << catPos.x << " " << catPos.y << std::endl;
  while (!frontier.empty()) {
    // get the current from frontier
    // remove the current from frontierset
    // mark current as visited
    // getVisitableNeightbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
    // iterate over the neighs:
    // for every neighbor set the cameFrom
    // enqueue the neighbors to frontier and frontierset
    // do this up to find a visitable border and break the loop

    Point2D current = frontier.back();

    if (current.x == w->getWorldSideSize()/2 || current.x == -1 * w->getWorldSideSize()/2 || current.y == w->getWorldSideSize()/2 || current.y == -1 * w->getWorldSideSize()/2)
    {
      borderExit = frontier.back();
      break;
    }

    frontierSet.erase(current);
    visited[current] = true;

    std::vector<Point2D> visitables = w->getVisitableNeighbors(current, frontier);
    for (int i = 0; i < visitables.size(); i++) std::cout << visitables[i].x << " " << visitables[i].y << std::endl;

    for (int i = 0; i < visitables.size(); i++)
    {
      if (cameFrom.count(visitables[i]) == 0)
      {
        frontier.push(visitables[i]);
        frontierSet.insert(visitables[i]);
        //cameFrom[visitables[i]] = current;
        cameFrom.insert({visitables[i], current});
        //cout << cameFrom[visitables[i]].x << ", " << cameFrom[visitables[i]].y << endl;
      }
    }

  }

  // if the border is not infinity, build the path from border to the cat using the camefrom map
  // if there isnt a reachable border, just return empty vector
  // if your vector is filled from the border to the cat, the first element is the catcher move, and the last element is the cat move

  std::vector<Point2D> path = vector<Point2D>();
  Point2D current = borderExit;
  //std::cout << current.x << " " << current.y << endl;

  if (borderExit != Point2D::INFINITE)
  {
    while (current != catPos)
    {
      path.push_back(current);
      current = cameFrom[current];
      //std::cout << "Current: " << current.x << " " << current.y << " Next: " << cameFrom[current].x << " " << cameFrom[current].y << " Cat: " << catPos.x << " " << catPos.y << std::endl;
    }
  }

  std::cout << std::endl;
  return path;
}

// todo (Liam): Current issue is that the Cat does not adapt when it is blocked off. If its target is blocked, it just sits in place and gets stuck in an infinite loop instead of searching for a new target. sometiems the cat gets stuck in a corner. It seems to refuse to search upwards or right, however it'll move right to dodge an obstacle.