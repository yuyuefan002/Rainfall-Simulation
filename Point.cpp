#include "Point.h"
#include <iostream>
Point::Point(const float absorptionrate,
             const std::vector<std::pair<int, int>> &points)
    : absorptionRate(absorptionrate), raindropRate(1), trickleAwayRate(1),
      currentDrops(0), absorbedDrops(0) {
  for (auto point : points) {
    lowerPoints.push_back(std::make_pair(point.first, point.second));
  }
}
void Point::rainfall() { currentDrops += raindropRate; }

void Point::absorb() {
  float absorbingDrops = fmin(absorptionRate, currentDrops);
  absorbedDrops += absorbingDrops;
  currentDrops -= absorbingDrops;
}

void Point::trickleAway(std::vector<std::vector<float>> &delta) {
  int numberOfLowerPoints = lowerPoints.size();
  if (numberOfLowerPoints == 0 || currentDrops == 0) {
    return;
  }

  float trickleAwayRaindropSize =
      fmin(trickleAwayRate, currentDrops) / numberOfLowerPoints;
  currentDrops -= trickleAwayRaindropSize * numberOfLowerPoints;

  for (auto pointIndexes : lowerPoints) {
    int i = pointIndexes.first;
    int j = pointIndexes.second;
    delta[i][j] += trickleAwayRaindropSize;
  }
}

void Point::trickleIn(const float trickleAwayRaindropSize) {
  currentDrops += trickleAwayRaindropSize;
}

bool Point::isClean() { return currentDrops == 0; }

float Point::reportAbsorbedDrops() { return absorbedDrops; }

float Point::reportCurrentDrops() { return currentDrops; }
