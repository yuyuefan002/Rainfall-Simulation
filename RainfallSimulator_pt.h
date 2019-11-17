#ifndef __RAINFALLSIMULATOR_PT_H__
#define __RAINFALLSIMULATOR_PT_H__
#include "Point_pt.h"
#include <chrono>
#include <vector>

template <typename T> class Landscape {
private:
  T *landscape;
  int size;

public:
  explicit Landscape(int n) {
    size = n;
    landscape = new T[n * n];
  }

  ~Landscape() { delete[] landscape; }

  T *operator[](int x) { return (landscape + x * size); }

  T *operator[](int x) const { return (landscape + x * size); }
};

struct thread_info {
  int num_thread;
  int raindropInterval;
  int size;
  Landscape<Point_pt> *landscape;
  pthread_mutex_t *mutex_array;
  int thread_id;
};

void *rainfall(void *arg);
void printResult(float duration, const Landscape<Point_pt> &landscape,
                 const int &size);
void initLandscape(Landscape<Point_pt> &landscape, const int &size,
                   std::vector<std::vector<int>> &elevations,
                   const float &absorptionRate);
#endif
