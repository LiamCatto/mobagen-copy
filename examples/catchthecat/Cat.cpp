#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world) {
  // auto rand = Random::Range(0, 5);
  // auto pos = world->getCat();
  // switch (rand) {
  //   case 0:
  //     return World::NE(pos);
  //   case 1:
  //     return World::NW(pos);
  //   case 2:
  //     return World::E(pos);
  //   case 3:
  //     return World::W(pos);
  //   case 4:
  //     return World::SW(pos);
  //   case 5:
  //     return World::SE(pos);
  //   default:
  //     throw "random out of range";
  // }

  //std::cout << generatePath(world).size() << std::endl;
  //std::cout << generatePath(world)[generatePath(world).size() - 1].x << " " << generatePath(world)[generatePath(world).size() - 1].y << std::endl;
  std::vector<Point2D> path = generatePath(world);
  int size = path.size();

  if (size > 0) return path[size - 1];
  else return path[0];
}
