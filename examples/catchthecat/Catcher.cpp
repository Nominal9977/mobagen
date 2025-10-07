#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) {
  auto path = generatePath(world);
  return path[path.size() - 1];
}
