#include "Agent.h"
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <cmath>
#include "World.h"

#include <algorithm>
using namespace std;

// A* uses a priority queue with f-score (g + h)
struct AStarNode {
  Point2D position;
  float fScore;  // g + h

  bool operator>(const AStarNode& other) const {
    return fScore > other.fScore;
  }
};

float heuristic(const Point2D& from, const Point2D& to) {
  int dx = abs(to.x - from.x);
  int dy = abs(to.y - from.y);
  // Hexagonal distance approximation
  return dx + max(0, (dy - dx) / 2);
}

std::vector<Point2D> Agent::generatePath(World* w) {
  unordered_map<Point2D, Point2D> cameFrom;
  unordered_map<Point2D, float> gScore;
  priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> openSet;
  unordered_set<Point2D> openSetLookup;
  unordered_set<Point2D> closedSet;

  auto catPos = w->getCat();
  int sideSize = w->getWorldSideSize();
  int halfSize = sideSize / 2;

  gScore[catPos] = 0;

  Point2D goalEstimate = catPos;
  float minDistToBorder = INFINITY;

  vector<Point2D> borderPoints;
  for (int i = -halfSize; i <= halfSize; i++) {
    borderPoints.push_back({-halfSize, i});
    borderPoints.push_back({halfSize, i});
    borderPoints.push_back({i, -halfSize});
    borderPoints.push_back({i, halfSize});
  }

  for (const auto& border : borderPoints) {
    float dist = heuristic(catPos, border);
    if (dist < minDistToBorder) {
      minDistToBorder = dist;
      goalEstimate = border;
    }
  }

  openSet.push({catPos, heuristic(catPos, goalEstimate)});
  openSetLookup.insert(catPos);

  Point2D borderExit = Point2D::INFINITE;

  while (!openSet.empty()) {

    AStarNode current = openSet.top();
    openSet.pop();
    openSetLookup.erase(current.position);

    if (w->catWinsOnSpace(current.position)) {
      borderExit = current.position;
      break;
    }

    closedSet.insert(current.position);

    auto neighs = World::neighbors(current.position);

    for (const auto& neighbor : neighs) {
      if (!w->isValidPosition(neighbor) ||
          w->getContent(neighbor) ||
          closedSet.count(neighbor)) {
        continue;
      }

      float tentativeGScore = gScore[current.position] + 1;

      if (!gScore.count(neighbor) || tentativeGScore < gScore[neighbor]) {
        cameFrom[neighbor] = current.position;
        gScore[neighbor] = tentativeGScore;
        float fScore = tentativeGScore + heuristic(neighbor, goalEstimate);

        if (!openSetLookup.count(neighbor)) {
          openSet.push({neighbor, fScore});
          openSetLookup.insert(neighbor);
        }
      }
    }
  }

  if (borderExit != Point2D::INFINITE) {
    vector<Point2D> path;
    Point2D current = borderExit;

    while (cameFrom.count(current)) {
      path.push_back(current);
      current = cameFrom[current];
    }

    reverse(path.begin(), path.end());
    return path;
  }

  return vector<Point2D>();
}