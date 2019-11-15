#include "Point_pt.h"
#include <iostream>
Point_pt::Point_pt(const float absorptionrate,
                   const std::vector<std::pair<int, int>> &points)
    : absorptionRate(absorptionrate), raindropRate(1), trickleAwayRate(1),
      currentDrops(0), absorbedDrops(0) {
  for (auto point : points) {
    lowerPoints.push_back(std::make_pair(point.first, point.second));
  }
}
void Point_pt::rainfall() { currentDrops += raindropRate; }

void Point_pt::absorb() {
  float absorbingDrops = std::min(absorptionRate, currentDrops);
  absorbedDrops += absorbingDrops;
  currentDrops -= absorbingDrops;
}

void Point_pt::trickleAway(
    std::vector<std::vector<float>> &delta,
    std::vector<std::vector<pthread_mutex_t>> &delta_locks) {
  int numberOfLowerPoint_pts = lowerPoints.size();
  if (numberOfLowerPoint_pts == 0 || currentDrops == 0) {
    return;
  }
  float totaltrickleAwayRaindropSize = std::min(trickleAwayRate, currentDrops);
  float trickleAwayRaindropSize =
      totaltrickleAwayRaindropSize / numberOfLowerPoint_pts;
  currentDrops -= totaltrickleAwayRaindropSize;
  for (auto pointIndexes : lowerPoints) {
    int i = pointIndexes.first;
    int j = pointIndexes.second;
    pthread_mutex_lock(&delta_locks[i][j]);
    delta[i][j] += trickleAwayRaindropSize;
    pthread_mutex_unlock(&delta_locks[i][j]);
  }
}

void Point_pt::trickleIn(const float trickleAwayRaindropSize) {
  currentDrops += trickleAwayRaindropSize;
}

bool Point_pt::isClean() { return currentDrops == 0; }

float Point_pt::reportAbsorbedDrops() { return absorbedDrops; }

float Point_pt::reportCurrentDrops() { return currentDrops; }
