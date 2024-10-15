#include "Agent.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "World.h"
using namespace std;

struct Agent::AStarNode {  // Written by my professor, Alexandre Tolstenko. Copied from code he gave to the class.
  AStarNode(Point2D p, int h, int a) : point(p), heuristic(h), accumulatedDistance(a){};
  AStarNode() = default;

  Point2D point = Point2D(0, 0);
  int heuristic = 0;
  int accumulatedDistance = 0;

  // operator less for the priority queue
  bool operator<(const AStarNode& other) const {
    // the operators are swapped because we are looking for the minimum
    return heuristic + accumulatedDistance > other.heuristic + other.accumulatedDistance;
  }
  // struct AStarLess {
  //   bool operator<(const AStarNode& left, const AStarNode& right) const {
  //     // the operators are swapped because we are looking for the minimum
  //     return left.heuristic + left.accumulatedDistance > right.heuristic + right.accumulatedDistance;
  //   }
  // };
};

int Agent::heuristic(Point2D p, int SideSizeOver2) {  // First case written by my professor, Alexandre Tolstenko. Copied from code he gave to the class.
  // Distance to right border
  if (p.x - p.y > 0 && p.x + p.y > 0) {
    return SideSizeOver2 - p.x;
  }
  // Distance to bottom border
  if (p.x - p.y > 0 && p.x + p.y < 0) {
    return SideSizeOver2 - p.y;
  }
  // Distance to left border
  if (p.x - p.y < 0 && p.x + p.y < 0) {
    return SideSizeOver2 + p.x;
  }
  // Distance to top border
  if (p.x - p.y < 0 && p.x + p.y > 0) {
    return SideSizeOver2 + p.y;
  }
  // Case where the agent is equally distant from all borders
  if (p.x == 0 && p.y == 0) {
    return SideSizeOver2;
  }
}

std::vector<Point2D> Agent::generatePath(World* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  queue<Point2D> frontier;                   // to store next ones to visit
  //priority_queue<Point2D, std::unordered_map<Point2D, AStarNode>, AStarNode::AStarLess> frontier;
  unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results

  // bootstrap state
  auto catPos = w->getCat();
  frontier.push(catPos);
  frontierSet.insert(catPos);
  Point2D borderExit = Point2D::INFINITE;  // if at the end of the loop we dont find a border, we have to return random points

  std::vector<Point2D> visitables;
  std::unordered_map<Point2D, AStarNode> nodeSet;
  int costPerTile = 1;
  nodeSet[catPos] = AStarNode(catPos, heuristic(catPos, w->getWorldSideSize()/2), 0);

  //std::cout << "catPos: " << catPos.x << " " << catPos.y << std::endl;
  while (!frontier.empty()) {
    // get the current from frontier
    // remove the current from frontierset
    // mark current as visited
    // getVisitableNeightbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
    // iterate over the neighs:
    // for every neighbor set the cameFrom
    // enqueue the neighbors to frontier and frontierset
    // do this up to find a visitable border and break the loop

    Point2D current = frontier.front();
    frontierSet.erase(current);
    visited[current] = true;

    if (current.x == w->getWorldSideSize()/2 || current.x == -1 * w->getWorldSideSize()/2 || current.y == w->getWorldSideSize()/2 || current.y == -1 * w->getWorldSideSize()/2)
    {
      borderExit = frontier.front();
      break;
    }

    visitables = w->getVisitableNeighbors(current, frontier);
    //for (int i = 0; i < visitables.size(); i++) std::cout << visitables[i].x << " " << visitables[i].y << std::endl;
    //std::cout << std::endl;

    for (int i = 0; i < visitables.size(); i++)
    {
      // if (cameFrom.count(visitables[i]) == 0)
      // {
      //   frontier.push(visitables[i]);
      //   frontierSet.insert(visitables[i]);
      //   //cameFrom[visitables[i]] = current;
      //   cameFrom.insert({visitables[i], current});
      //   //cout << cameFrom[visitables[i]].x << ", " << cameFrom[visitables[i]].y << endl;
      // }

      int newCost = costPerTile + nodeSet[current].accumulatedDistance;
      if (nodeSet.count(visitables[i]) == 0 || newCost < nodeSet[visitables[i]].accumulatedDistance)
      {
        int distToBorder = heuristic(visitables[i], w->getWorldSideSize()/2);
        nodeSet.emplace(visitables[i], AStarNode(visitables[i], distToBorder, newCost));

        frontier.push(visitables[i]);
        frontierSet.insert(visitables[i]);
        //cameFrom[visitables[i]] = current;
        cameFrom.insert({visitables[i], current});
        //cout << cameFrom[visitables[i]].x << ", " << cameFrom[visitables[i]].y << endl;
      }
    }

    frontier.pop();
  }

  // if the border is not infinity, build the path from border to the cat using the camefrom map
  // if there isnt a reachable border, just return empty vector
  // if your vector is filled from the border to the cat, the first element is the catcher move, and the last element is the cat move

  std::vector<Point2D> path = vector<Point2D>();
  Point2D current = borderExit;
  //std::cout << current.x << " " << current.y << endl;

  if (borderExit != Point2D::INFINITE)    // If there is a visitable border, create a path to that border.
  {
    while (current != catPos)
    {
      path.push_back(current);
      current = cameFrom[current];
      //std::cout << "Current: " << current.x << " " << current.y << " Next: " << cameFrom[current].x << " " << cameFrom[current].y << " Cat: " << catPos.x << " " << catPos.y << std::endl;
    }
  }
  else if (visitables.size() > 0)   // If a border cannot be found, then create a path to a random visitable neighbor
  {
    srand(time(NULL));
    int randomNum = rand() % (visitables.size() - 1);
    path.push_back(visitables[randomNum]);
  }
  else    // If the cat is unable to move, end the game by moving outside the world bounds (and therefore more than 1 tile)
  {
    path.push_back(Point2D(w->getWorldSideSize(), w->getWorldSideSize()));
  }

  //std::cout << std::endl << std::endl << std::endl;
  //if (path.size() == 0) std::cout << borderExit.x << " " << borderExit.y << std::endl;
  return path;
}

// todo (Liam): switch from queue to priority queue, with the priority based on the AStarNode less operator. Maybe make a priorityQueue of AStarNodes and push each point as a node?