#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world) {
  auto path = generatePath(world);
  if(path.size() == 0) {
    return Point2D(0, 0);
  }
  return path[0];
}
