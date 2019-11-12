#include "RainfallSimulator.cpp"
#include "Point.cpp"
#include <assert.h>
#include <iostream>
class RainfallSimulatorTest : protected RainfallSimulator {
public:
  RainfallSimulatorTest(const std::vector<std::vector<int>> &elevations,
                        const int &N, const int &M, const float &A)
      : RainfallSimulator(elevations, N, M, A) {}

  void isCompleteTest() {
    this->runOneTimestamp();
    assert(this->isComplete());
    std::cout << "Test: isCompleteTest passed\n";
  }

  void runOneTimeStampTest() {
    this->runOneTimestamp();
    assert(this->landscape[0][1].isClean());
    assert(!this->landscape[0][0].isClean());
    assert(this->landscape[1][1].isClean());
    assert(!this->landscape[1][0].isClean());
    std::cout << "Test: runOneTimeStampTest passed\n";
  }
};

std::vector<std::vector<int>> giveMeAnElevations() {
  std::vector<std::vector<int>> elevation(2);
  for (int i = 0; i < 2; i++) {
    elevation[i].push_back(0);
    elevation[i].push_back(1);
  }
  return elevation;
}
int main() {
  std::vector<std::vector<int>> elevation = giveMeAnElevations();
  RainfallSimulatorTest rainfallSimulatorTest1(elevation, 1, 1, 1);
  rainfallSimulatorTest1.isCompleteTest();

  RainfallSimulatorTest rainfallSimulatorTest2(elevation, 2, 1, 0.5);
  rainfallSimulatorTest2.runOneTimeStampTest();
}
