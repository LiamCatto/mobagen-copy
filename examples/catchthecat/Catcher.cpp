#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) {
  auto side = world->getWorldSideSize() / 2;
  // for (;;) {
  //   Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
  //   auto cat = world->getCat();
  //   if (cat.x != p.x && cat.y != p.y && !world->getContent(p)) return p;
  // }

  //std::cout << generatePath(world)[0].x << " " << generatePath(world)[0].y << std::endl;
  return generatePath(world)[0];
  //return Point2D(0, 0);
}
