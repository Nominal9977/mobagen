#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) {
  auto path = generatePath(world);
  if(path.size() == 0) {
    return Point2D(0, 0);
  }
  return path[path.size() - 1];
}
