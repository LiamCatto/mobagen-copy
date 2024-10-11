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

  while (!frontier.empty()) {
    // get the current from frontier
    // remove the current from frontierset
    // mark current as visited
    // getVisitableNeightbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
    // iterate over the neighs:
    // for every neighbor set the cameFrom
    // enqueue the neighbors to frontier and frontierset
    // do this up to find a visitable border and break the loop

    frontierSet.erase(frontier.back());
    visited[frontier.back()] = true;

    std::vector<Point2D> visitables = w->getVisitableNeighbors(frontier.back(), frontier);
    //for (int i = 0; i < visitables.size(); i++) std::cout << visitables[i].x << " " << visitables[i].y << std::endl;

    for (int i = 0; i < visitables.size(); i++)
    {
      if (cameFrom[visitables[i]] == Point2D(0, 0))
      {
        //cout << cameFrom[visitables[i]].x << ", " << cameFrom[visitables[i]].y << endl;
        cameFrom[visitables[i]] = frontier.back();
        frontier.push(visitables[i]);
        frontierSet.insert(visitables[i]);
      }
    }

    if (frontier.back().x == w->getWorldSideSize()/2 || frontier.back().x == -1 * w->getWorldSideSize()/2 || frontier.back().y == w->getWorldSideSize()/2 || frontier.back().y == -1 * w->getWorldSideSize()/2)
    {
      borderExit = frontier.back();
      break;
    }
  }

  // if the border is not infinity, build the path from border to the cat using the camefrom map
  // if there isnt a reachable border, just return empty vector
  // if your vector is filled from the border to the cat, the first element is the catcher move, and the last element is the cat move

  std::vector<Point2D> path = vector<Point2D>();
  Point2D current = borderExit;

  if (borderExit != Point2D::INFINITE)
  {
    while (current != catPos)
    {
      path.push_back(current);
      current = cameFrom[current];
      //std::cout << current.x << " " << current.y << ", " << catPos.x << " " << catPos.y << std::endl;
    }
  }

  return path;
}

// todo (Liam): Current issue seems to be catcher getting stuck in an infinite loop while the path is being reconstructed.