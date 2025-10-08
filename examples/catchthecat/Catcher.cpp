#include "Catcher.h"
#include "World.h"
#include "Agent.h"
#include <cmath>
#include <limits>

Point2D Catcher::Move(World* pWorld) {
    Point2D catPos = pWorld->getCat();

    auto catPath = generatePath(pWorld);

    if (catPath.empty()) {
        auto catNeighbors = World::neighbors(catPos);
        std::vector<Point2D> validPositions;

        for (const auto& n : catNeighbors) {
            if (pWorld->catcherCanMoveToPosition(n) && !pWorld->getContent(n)) {
                validPositions.push_back(n);
            }
        }

        if (!validPositions.empty()) {
            int randomIndex = Random::Range(0, (int)validPositions.size() - 1);
            return validPositions[randomIndex];
        }

        return Point2D::INFINITE;
    }

    Point2D bestBlock = Point2D::INFINITE;
    float bestScore = -std::numeric_limits<float>::infinity();

    for (size_t i = 0; i < catPath.size(); i++) {
        Point2D pathPos = catPath[i];

        if (!pWorld->catcherCanMoveToPosition(pathPos)) continue;

        float score = 0;

        score += (catPath.size() - i) * 5;

        auto neighbors = World::neighbors(pathPos);
        int blockedNeighbors = 0;
        for (const auto& n : neighbors) {
            if (!pWorld->isValidPosition(n) || pWorld->getContent(n)) {
                blockedNeighbors++;
            }
        }
        score += blockedNeighbors * 5;

        if (score > bestScore) {
            bestScore = score;
            bestBlock = pathPos;
        }
    }

  if (bestBlock == Point2D::INFINITE) {
    for (const auto& pathPos : catPath) {
      auto neighbors = World::neighbors(pathPos);
      for (const auto& n : neighbors) {
        if (pWorld->catcherCanMoveToPosition(n) && !pWorld->getContent(n)) {
          return n;
        }
      }
    }
  }

  if (bestBlock == Point2D::INFINITE) {
   return catPath[catPath.size() - 1];
  }

    return bestBlock;
}