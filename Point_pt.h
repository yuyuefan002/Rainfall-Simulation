#ifndef __POINT_PT_H__
#define __POINT_PT_H__

#include <algorithm>
#include <climits>
#include <iostream>
class Point_pt {
private:
  float absorptionRate;
  float trickleAwayRate;
  float currentDrops;
  float absorbedDrops;
  bool willTrickle;

public:
  float trickleAmount;
  int trickleNumber;
  bool topTrickle;
  bool bottomTrickle;
  bool leftTrickle;
  bool rightTrickle;
  void setWillTrickle(bool willtrickle);
  bool willTrickle_();
  bool notrickle();
  void absorb();
  void trickleIn(float &amount);
  void trickleAway();
  int absorbAll();
  Point_pt() : trickleAwayRate(1) {}
  void setAbsorptionRate(const float &abosorptionRate);
  float reportAbsorbedDrops();
};

#endif
