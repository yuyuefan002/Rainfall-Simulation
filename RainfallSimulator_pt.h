#ifndef __RAINFALLSIMULATOR_PT_H__
#define __RAINFALLSIMULATOR_PT_H__
#include "Point_pt.h"
#include <chrono>
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <vector>
class RainfallSimulator_pt {
protected:
  int num_thread;
  int size;
  int raindropInterval;
  int absorptionRate;
  int operationTime;
  bool complete;
  std::vector<std::vector<Point_pt>> landscape;
  void check(std::vector<std::pair<int, int>> &lowestNeighbor,
             int &min_elevation, const int &current_elevation, const int &i,
             const int &j, const std::vector<std::vector<int>> &elevations);
  std::vector<std::pair<int, int>>
  checkLowestNeighbor(const std::vector<std::vector<int>> &elevations, int i,
                      int j);

public:
  RainfallSimulator_pt(const std::vector<std::vector<int>> &elevations,
                       const int &P, const int &N, const int &M,
                       const float &A);
  void printAbsorbedRainDrops();
  void printCurrentRainDrops();
  void runWholeProcess(std::vector<std::vector<pthread_mutex_t>> &mutex,
                       pthread_barrier_t &barrier);
};

struct thread_info {
  int start;
  int end;
  int size;
  int raindropInterval;
  std::vector<std::vector<float>> *delta;
  std::vector<std::vector<Point_pt>> *landscape;
  pthread_barrier_t *barrier;
  std::vector<std::vector<pthread_mutex_t>> *delta_locks;
};
#endif
