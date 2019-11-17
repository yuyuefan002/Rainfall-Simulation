#ifndef __POINT_H__
#define __POINT_H__
#include <math.h>
#include <utility>
#include <vector>

class Point {
protected:
  float absorptionRate;
  float raindropRate;
  float trickleAwayRate;
  float currentDrops;
  float absorbedDrops;
  std::vector<std::pair<int, int>> lowerPoints;

public:
  Point();
  Point(const float absorptionrate,
        const std::vector<std::pair<int, int>> &points);
  void rainfall();
  void absorb();
  void trickleIn(const float trickleAwayRaindropSize);
  void trickleAway(std::vector<std::vector<float>> &delta);
  bool isClean();
  float reportAbsorbedDrops();
  float reportCurrentDrops();
};

#endif
