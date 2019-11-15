#ifndef __POINT_PT_H__
#define __POINT_PT_H__
#include <math.h>
#include <utility>
#include <vector>

class Point_pt {
protected:
  float absorptionRate;
  float raindropRate;
  float trickleAwayRate;
  float currentDrops;
  float absorbedDrops;
  std::vector<std::pair<int, int>> lowerPoints;

public:
  Point_pt();
  Point_pt(const float absorptionrate,
           const std::vector<std::pair<int, int>> &points);
  void rainfall();
  void absorb();
  void trickleIn(const float trickleAwayRaindropSize);
  void trickleAway(std::vector<std::vector<float>> &delta,
                   std::vector<std::vector<pthread_mutex_t>> &delta_locks);
  bool isClean();
  float reportAbsorbedDrops();
  float reportCurrentDrops();
};

#endif
