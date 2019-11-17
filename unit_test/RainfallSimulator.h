#ifndef __RAINFALLSIMULATOR_H__
#define __RAINFALLSIMULATOR_H__
#include "Point.h"
#include <iostream>
#include <math.h>
#include <vector>

class RainfallSimulator {
protected:
  int size;
  int raindropInterval;
  int absorptionRate;
  int operationTime;
  bool complete;
  std::vector<std::vector<Point>> landscape;
  void check(std::vector<std::pair<int, int>> &lowestNeighbor,
             int &min_elevation, const int &current_elevation, const int &i,
             const int &j, const std::vector<std::vector<int>> &elevations);
  std::vector<std::pair<int, int>>
  checkLowestNeighbor(const std::vector<std::vector<int>> &elevations, int i,
                      int j);

public:
  RainfallSimulator(const std::vector<std::vector<int>> &elevations,
                    const int &N, const int &M, const float &A);
  void runOneTimestamp();
  bool isComplete();
  void printOperationTime();
  void printAbsorbedRainDrops();
  void printCurrentRainDrops();
};

#endif
