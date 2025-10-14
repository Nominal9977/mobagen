#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(World* world) {
  auto path = generatePath(world);
  if (path.empty()) {
    Point2D catPos = world->getCat();
    auto catNeighbors = World::neighbors(catPos);
    std::vector<Point2D> validPositions;

    for (const auto& n : catNeighbors) {
      if (world->catcherCanMoveToPosition(n) && !world->getContent(n)) {
        validPositions.push_back(n);
      }
    }

    if (!validPositions.empty()) {
      int randomIndex = Random::Range(0, (int)validPositions.size() - 1);
      return validPositions[randomIndex];
    }

    return Point2D::INFINITE;
  }
  return path[0];
}
