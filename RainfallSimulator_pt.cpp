#include "RainfallSimulator_pt.h"
bool notrickle = false;
int operationTime = 0;
pthread_barrier_t barrier;

void check(std::vector<std::pair<int, int>> &lowestNeighbor, int &min_elevation,
           const int &current_elevation, const int &i, const int &j,
           const std::vector<std::vector<int>> &elevations) {
  int size = elevations.size();
  if (i >= size || i < 0)
    return;
  if (j >= size || j < 0)
    return;
  if (elevations[i][j] < min_elevation) {
    min_elevation = elevations[i][j];
    lowestNeighbor.clear();
    lowestNeighbor.push_back(std::make_pair(i + 1, j + 1));
  } else if (elevations[i][j] == min_elevation &&
             elevations[i][j] != current_elevation) {
    lowestNeighbor.push_back(std::make_pair(i + 1, j + 1));
  }
}

std::vector<std::pair<int, int>>
checkLowestNeighbor(const std::vector<std::vector<int>> &elevations, int i,
                    int j) {
  std::vector<std::pair<int, int>> lowestNeighbor;
  int min_elevation = elevations[i][j];
  check(lowestNeighbor, min_elevation, elevations[i][j], i - 1, j, elevations);
  check(lowestNeighbor, min_elevation, elevations[i][j], i, j - 1, elevations);
  check(lowestNeighbor, min_elevation, elevations[i][j], i, j + 1, elevations);
  check(lowestNeighbor, min_elevation, elevations[i][j], i + 1, j, elevations);
  return lowestNeighbor;
}
void updateEachPoint(Landscape<Point_pt> &landscape, int i, int j,
                     const std::vector<std::pair<int, int>> &lowerPoints) {
  if (lowerPoints.size() == 0) {
    landscape[i][j].setWillTrickle(false);
    return;
  }
  landscape[i][j].setWillTrickle(true);
  landscape[i][j].trickleNumber = lowerPoints.size();
  for (auto lowerpoint : lowerPoints) {
    int lower_i = lowerpoint.first;
    int lower_j = lowerpoint.second;
    if (lower_i == i - 1 && lower_j == j) {
      landscape[i][j].topTrickle = true;
    } else if (lower_i == i + 1 && lower_j == j) {
      landscape[i][j].bottomTrickle = true;
    } else if (lower_i == i && lower_j == j - 1) {
      landscape[i][j].leftTrickle = true;
    } else if (lower_i == i && lower_j == j + 1) {
      landscape[i][j].rightTrickle = true;
    }
  }
}
void initLandscape(Landscape<Point_pt> &landscape, const int &size,
                   std::vector<std::vector<int>> &elevations,
                   const float &absorptionRate) {
  for (int i = 1; i < size + 1; ++i) {
    for (int j = 1; j < size + 1; ++j) {
      std::vector<std::pair<int, int>> lowerPoints =
          checkLowestNeighbor(elevations, i - 1, j - 1);
      updateEachPoint(landscape, i, j, lowerPoints);
      landscape[i][j].setAbsorptionRate(absorptionRate);
    }
  }
}

bool noTrickle(Landscape<Point_pt> &landscape, const int &N) {
  for (int i = 1; i < N + 1; ++i) {
    for (int j = 1; j < N + 1; ++j) {
      if (!landscape[i][j].notrickle()) {
        return false;
      }
    }
  }

  int maxAbsorptionTime = 0;
  for (int i = 1; i < N + 1; ++i) {
    for (int j = 1; j < N + 1; ++j) {
      maxAbsorptionTime =
          std::max(maxAbsorptionTime, landscape[i][j].absorbAll());
    }
  }

  operationTime += maxAbsorptionTime;
  return true;
}

void rainAndAbsorbAndTrickleAway(Landscape<Point_pt> &landscape,
                                 const int &operationTime, const int &size,
                                 const int &raindropInterval, const int &start,
                                 const int &end) {
  for (int i = start; i < end; i++) {
    for (int j = 1; j < size + 1; j++) {
      if (operationTime < raindropInterval) {
        float rainDropRate = 1;
        landscape[i][j].trickleIn(rainDropRate);
      }
      landscape[i][j].absorb();
      landscape[i][j].trickleAway();
    }
  }
}
void trickleIn(Landscape<Point_pt> &landscape, pthread_mutex_t *&mutex_array,
               const int &size, const int &start, const int &end) {
  int first_row = start;
  int second_row = start + 1;
  int last_row = end - 1;
  int second_last_row = end - 2;
  for (int i = start; i < end; i++) {
    for (int j = 1; j < size + 1; j++) {
      if (!landscape[i][j].willTrickle_()) {
        continue;
      }
      float trickleAwayRaindropSize =
          (landscape[i][j].trickleAmount / landscape[i][j].trickleNumber);
      if (landscape[i][j].topTrickle) {
        if (i == first_row || i == second_row) {
          pthread_mutex_lock(&mutex_array[i - 1]);
          landscape[i - 1][j].trickleIn(trickleAwayRaindropSize);
          pthread_mutex_unlock(&mutex_array[i - 1]);
        } else {
          landscape[i - 1][j].trickleIn(trickleAwayRaindropSize);
        }
      }
      if (landscape[i][j].bottomTrickle) {
        if (i == last_row || i == second_last_row) {
          pthread_mutex_lock(&mutex_array[i + 1]);
          landscape[i + 1][j].trickleIn(trickleAwayRaindropSize);
          pthread_mutex_unlock(&mutex_array[i + 1]);
        } else {
          landscape[i + 1][j].trickleIn(trickleAwayRaindropSize);
        }
      }
      if (landscape[i][j].leftTrickle) {
        if (i == first_row || i == last_row) {
          pthread_mutex_lock(&mutex_array[i]);
          landscape[i][j - 1].trickleIn(trickleAwayRaindropSize);
          pthread_mutex_unlock(&mutex_array[i]);
        } else {
          landscape[i][j - 1].trickleIn(trickleAwayRaindropSize);
        }
      }
      if (landscape[i][j].rightTrickle) {
        if (i == first_row || i == last_row) {
          pthread_mutex_lock(&mutex_array[i]);
          landscape[i][j + 1].trickleIn(trickleAwayRaindropSize);
          pthread_mutex_unlock(&mutex_array[i]);
        } else {
          landscape[i][j + 1].trickleIn(trickleAwayRaindropSize);
        }
      }
    }
  }
}

void *rainfall(void *arg) {
  struct thread_info info = *((struct thread_info *)arg);
  int size = info.size;
  int thread_id = info.thread_id;
  int num_thread = info.num_thread;
  int raindropInterval = info.raindropInterval;
  pthread_mutex_t *mutex_array = info.mutex_array;
  Landscape<Point_pt> &landscape = *info.landscape;

  int granularity = size / num_thread;
  int start = thread_id * granularity + 1;
  int end = start + granularity;

  while (!notrickle) {
    rainAndAbsorbAndTrickleAway(landscape, operationTime, size,
                                raindropInterval, start, end);
    pthread_barrier_wait(&barrier);

    trickleIn(landscape, mutex_array, size, start, end);

    if (thread_id == 0) {
      operationTime++;
      notrickle = noTrickle(landscape, size);
    }

    pthread_barrier_wait(&barrier);
  }

  delete (struct thread_info *)arg;
  pthread_exit(NULL);
}

void printResult(float duration, const Landscape<Point_pt> &landscape,
                 const int &size) {
  fprintf(stdout, "Rainfall simulation took %d time steps to complete.\n",
          operationTime);
  std::cout << "Runtime = " << duration / 1e6 << " seconds\n";

  for (int i = 1; i < size + 1; ++i) {
    for (int j = 1; j < size + 1; ++j) {
      std::cout.width(8);
      std::cout << landscape[i][j].reportAbsorbedDrops();
    }
    std::cout << std::endl;
  }
}
