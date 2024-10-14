#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) {
  auto sideOver2 = world->getWorldSideSize() / 2;
  // for (;;) {
  //   Point2D p = {Random::Range(-side, side), Random::Range(-side, side)};
  //   auto cat = world->getCat();
  //   if (cat.x != p.x && cat.y != p.y && !world->getContent(p)) return p;
  // }

  //std::cout << generatePath(world)[0].x << " " << generatePath(world)[0].y << std::endl;

  // Catcher algorithm 1 (always wins)
  //return generatePath(world)[0];

  // Catcher algorithm 2 (more similar to a player)
  std::vector<Point2D> path = generatePath(world);
  int size = path.size();
  int count = 0;
  Point2D result;
  srand(time(NULL));

  if (size > 1) result = path[size - 2];    // Catcher blocks the positions one space ahead of the cat
  else if (size > 0) result = path[size - 1];   // Catcher blocks the positions two spaces ahead of the cat
  else    // Catcher blocks a random position
  {
     std::cout << "hi" << std::endl;
     while (!world->catcherCanMoveToPosition(result))    // If the blocker picks and invalid position, pick another one
     {
       result == Point2D(rand() - sideOver2 % (sideOver2 * 2), rand() - sideOver2 % (sideOver2 * 2));
       count++;

       if (count > (sideOver2 * 2) * (sideOver2 * 2)) break;   // If there are no valid places for the catcher to block, let the catcher make an invalid move to end the game
     }
   }

  return result;
}

// todo (Liam): show both catcher algorithms