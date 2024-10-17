#include "Agent.h"
#include <unordered_set>
#include <unordered_map>
#include "World.h"
using namespace std;


int Agent::calcHeuristic(Point2D p, int SideSizeOver2) {  // First case written by my professor, Alexandre Tolstenko. Copied from code he gave to the class.
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
  //if (p.x == 0 && p.y == 0) {
  return SideSizeOver2;
  //}
}

std::vector<Point2D> Agent::generatePath(World* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  //queue<Point2D> frontier;                   // to store next ones to visit
  priority_queue<AStarNode> frontier;
  unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results

  // bootstrap state
  auto catPos = w->getCat();
  frontier.emplace(catPos, 0 , 0);
  //frontierSet.insert(catPos);
  Point2D borderExit = Point2D::INFINITE;  // if at the end of the loop we dont find a border, we have to return random points

  std::vector<AStarNode> visitables;
  std::unordered_map<Point2D, AStarNode> nodeSet;
  //std::unordered_set<AStarNode> nodeSet;
  int costPerTile = 1;
  nodeSet[catPos] = AStarNode(catPos, calcHeuristic(catPos, w->getWorldSideSize()/2), 0);

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

    AStarNode current = frontier.top();
    //frontierSet.erase(current);
    visited[current.point] = true;

    if (current.point.x == w->getWorldSideSize()/2 || current.point.x == -1 * w->getWorldSideSize()/2 || current.point.y == w->getWorldSideSize()/2 || current.point.y == -1 * w->getWorldSideSize()/2)
    {
      borderExit = frontier.top().point;
      break;
    }

    visitables = getVisitableNeighbors(w, current, frontier, costPerTile);
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

      int newCost = costPerTile + current.accumulatedDistance;
      if (!nodeSet.contains(visitables[i].point) || newCost < nodeSet[visitables[i].point].accumulatedDistance)
      {
        //int distToBorder = heuristic(visitables[i].point, w->getWorldSideSize()/2);
        //nodeSet.emplace(visitables[i].point, AStarNode(visitables[i], distToBorder, newCost));
        nodeSet[visitables[i].point].accumulatedDistance = newCost;

        //frontier.emplace(visitables[i], distToBorder, newCost);
        frontier.push(visitables[i]);
        //frontierSet.insert(visitables[i]);
        //cameFrom[visitables[i]] = current;
        cameFrom[visitables[i].point] = current.point;
        //cout << frontier.top().point.x << ", " << frontier.top().point.y << " Priority: " << frontier.top().heuristic + frontier.top().accumulatedDistance << endl;  // Should be showing the priority for each node added to the frontier
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
  else if (visitables.size() > 1)   // If a border cannot be found, then create a path to a random visitable neighbor
  {
    srand(time(NULL));
    int randomNum = rand() % (visitables.size() - 1);
    path.push_back(visitables[randomNum].point);
  }
  else    // If the cat is unable to move, end the game by moving outside the world bounds (and therefore more than 1 tile)
  {
    path.push_back(Point2D(w->getWorldSideSize(), w->getWorldSideSize()));
  }

  //std::cout << std::endl << std::endl << std::endl;
  //if (path.size() == 0) std::cout << borderExit.x << " " << borderExit.y << std::endl;
  return path;
}

std::vector<AStarNode> Agent::getVisitableNeighbors(World* w, AStarNode node, std::priority_queue<AStarNode> frontier, int costPerTile)
{
  std::vector<Point2D> neighborList = World::neighbors(node.point);
  std::vector<AStarNode> result;

  for (int i = 0; i < neighborList.size(); i++)
  {
    if (neighborList[i] != w->getCat() && !w->getContent(neighborList[i])) {
      result.emplace_back(neighborList[i], calcHeuristic(node.point, w->getWorldSideSize()/2), node.accumulatedDistance + costPerTile);
    }
  }

  return result;
}

// todo (Liam): I'm not sure if the queue is following the correct priority. Sometimes the cat moves away from what is clearly the nearest border. Will ask about it in class.