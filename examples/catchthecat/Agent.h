#ifndef AGENT_H
#define AGENT_H
#include "math/Point2D.h"
#include <vector>

#include <stdlib.h>
#include <time.h>

class World;

class Agent {
private:

  struct AStarNode;
  int heuristic(Point2D p, int SideSizeOver2);

public:

  explicit Agent() = default;

  virtual Point2D Move(World*) = 0;

  std::vector<Point2D> generatePath(World* w);
};

#endif  // AGENT_H
