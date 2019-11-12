#include "Point.cpp"
#include <assert.h>
#include <iostream>

class PointTest : protected Point {
public:
  PointTest(const float absorptionrate,
            const std::vector<std::pair<int, int>> &points)
      : Point(absorptionrate, points) {}
  void rainfallTest() {
    this->rainfall();
    assert(this->currentDrops == 1);
    std::cout << "Test: rainfallTest passed\n";
  }

  void absorbMustNotAbsorbIfNoDrops() {
    this->absorb();
    assert(this->currentDrops == 0);
    assert(this->absorbedDrops == 0);
    std::cout << "Test: absorbMustNotAbsorbIfNoDropsTest passed\n";
  }

  void absorbMustAbsorbIfHaveDrops() {
    this->rainfall();
    this->absorb();
    assert(this->absorbedDrops == 1);
    assert(this->currentDrops == 0);
    std::cout << "Test: absorbMustAbsorbIfHaveDropsTest passed\n";
  }

  void trickleAwayMustTrickleDropToLowestPoint() {
    std::vector<std::vector<float>> delta(2, std::vector<float>(2, 0));
    this->rainfall();
    this->trickleAway(delta);
    assert(this->absorbedDrops == 0);
    assert(this->currentDrops == 0);
    assert(delta[0][1] == 0.5);
    assert(delta[1][0] == 0.5);
    std::cout << "Test: trickleAwayMustTrickleDropToLowestPoint passed\n";
  }

  void trickleAwayMustNotTrickleDropToLowestPointIfNoDrop() {
    std::vector<std::vector<float>> delta(2, std::vector<float>(2, 0));
    this->trickleAway(delta);
    assert(this->absorbedDrops == 0);
    assert(this->currentDrops == 0);
    assert(delta[0][1] == 0);
    std::cout
        << "Test: trickleAwayMustNotTrickleDropToLowestPointIfNoDrop passed\n";
  }

  void trickleInTest() {
    this->trickleIn(1);
    assert(this->currentDrops == 1);
    std::cout << "Test: trickleInTest passed\n";
  }

  void isCleanTest() {
    this->rainfall();
    this->absorb();
    assert(this->isClean());
    std::cout << "Test: isCleanTest passed\n";
  }
};

Point giveMeAPoint() {
  std::vector<std::pair<int, int>> lowerPoints;
  lowerPoints.push_back(std::make_pair(0, 1));
  Point point(1, lowerPoints);
  return point;
}

int main() {
  std::vector<std::pair<int, int>> lowerPoints;
  lowerPoints.push_back(std::make_pair(0, 1));

  std::vector<std::pair<int, int>> lowerPoints2;
  lowerPoints2.push_back(std::make_pair(0, 1));
  lowerPoints2.push_back(std::make_pair(1, 0));

  PointTest pointTest1(1, lowerPoints);
  pointTest1.absorbMustAbsorbIfHaveDrops();

  PointTest pointTest2(1, lowerPoints);
  pointTest2.rainfallTest();

  PointTest pointTest3(1, lowerPoints);
  pointTest3.absorbMustNotAbsorbIfNoDrops();

  PointTest pointTest4(1, lowerPoints);
  pointTest4.isCleanTest();

  PointTest pointTest5(1, lowerPoints2);
  pointTest5.trickleAwayMustTrickleDropToLowestPoint();

  PointTest pointTest6(1, lowerPoints);
  pointTest6.trickleInTest();

  PointTest pointTest7(1, lowerPoints);
  pointTest7.trickleAwayMustNotTrickleDropToLowestPointIfNoDrop();
}
