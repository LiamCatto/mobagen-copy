#ifndef AGENT_H
#define AGENT_H
#include "math/Point2D.h"
#include <vector>

#include <stdlib.h>
#include <time.h>

class World;

struct AStarNode {
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

class Agent {
public:

  explicit Agent() = default;

  virtual Point2D Move(World*) = 0;

  std::vector<Point2D> generatePath(World* w);

  int heuristic(Point2D p, int SideSizeOver2);
  std::vector<AStarNode> getVisitableNeighbors(AStarNode node, std::priority_queue<AStarNode> frontier, int costPerTile)
};



#endif  // AGENT_H
