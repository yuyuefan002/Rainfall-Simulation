#include "Point_pt.h"

bool Point_pt::notrickle() { return trickleAmount == 0; }

void Point_pt::trickleIn(float &amount) { currentDrops += amount; }

void Point_pt::absorb() {
  if (currentDrops <= 0) {
    return;
  }
  float absorbingDrops = std::min(absorptionRate, currentDrops);
  currentDrops -= absorbingDrops;
  absorbedDrops += absorbingDrops;
}
void Point_pt::setAbsorptionRate(const float &absorptionrate) {
  absorptionRate = absorptionrate;
}

void Point_pt::trickleAway() {
  if (!willTrickle || currentDrops <= 0) {
    trickleAmount = 0;
    return;
  }
  float totaltrickleAwayDropSize = std::min(trickleAwayRate, currentDrops);
  trickleAmount = totaltrickleAwayDropSize;
  currentDrops -= totaltrickleAwayDropSize;
}

int Point_pt::absorbAll() {
  if (currentDrops <= 0) {
    return 0;
  }
  float absorbingDrops = currentDrops;
  currentDrops = 0;
  absorbedDrops += absorbingDrops;
  return absorbingDrops / absorptionRate;
}

float Point_pt::reportAbsorbedDrops() { return absorbedDrops; }
void Point_pt::setWillTrickle(bool willtrickle) { willTrickle = willtrickle; }

bool Point_pt::willTrickle_() { return willTrickle; }
