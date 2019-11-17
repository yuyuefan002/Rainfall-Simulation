#include "RainfallSimulator_pt.h"

void RainfallSimulator_pt::check(
    std::vector<std::pair<int, int>> &lowestNeighbor, int &min_elevation,
    const int &current_elevation, const int &i, const int &j,
    const std::vector<std::vector<int>> &elevations) {
  if (i >= size || i < 0)
    return;
  if (j >= size || j < 0)
    return;
  if (elevations[i][j] < min_elevation) {
    min_elevation = elevations[i][j];
    lowestNeighbor.clear();
    lowestNeighbor.push_back(std::make_pair(i, j));
  } else if (elevations[i][j] == min_elevation &&
             elevations[i][j] != current_elevation) {
    lowestNeighbor.push_back(std::make_pair(i, j));
  }
}
std::vector<std::pair<int, int>> RainfallSimulator_pt::checkLowestNeighbor(
    const std::vector<std::vector<int>> &elevations, int i, int j) {
  std::vector<std::pair<int, int>> lowestNeighbor;
  int min_elevation = elevations[i][j];
  check(lowestNeighbor, min_elevation, elevations[i][j], i - 1, j, elevations);
  check(lowestNeighbor, min_elevation, elevations[i][j], i, j - 1, elevations);
  check(lowestNeighbor, min_elevation, elevations[i][j], i, j + 1, elevations);
  check(lowestNeighbor, min_elevation, elevations[i][j], i + 1, j, elevations);
  return lowestNeighbor;
}

RainfallSimulator_pt::RainfallSimulator_pt(
    const std::vector<std::vector<int>> &elevations, const int &P, const int &N,
    const int &M, const float &A)
    : num_thread(P), size(N), raindropInterval(M), absorptionRate(A),
      operationTime(0), complete(false) {
  for (int i = 0; i < size; ++i) {
    landscape.push_back(std::vector<Point_pt>());
    for (int j = 0; j < size; ++j) {
      std::vector<std::pair<int, int>> lowerPoints =
          checkLowestNeighbor(elevations, i, j);
      landscape[i].push_back(Point_pt(A, lowerPoints));
    }
  }
}
void normal_rainAndAbosrbAndTirckleAway(
    int &raindropInterval, int &start, int &end, int &size,
    std::vector<std::vector<Point_pt>> &landscape,
    std::vector<std::vector<float>> &delta,
    std::vector<std::vector<pthread_mutex_t>> *delta_locks) {
  if (raindropInterval != 0) {
    --raindropInterval;
    for (int i = start; i < end; ++i) {
      for (int j = 0; j < size; ++j) {
        landscape[i][j].rainfall();
        landscape[i][j].absorb();
        landscape[i][j].trickleAway(delta, *delta_locks);
      }
    }
  } else {
    for (int i = start; i < end; ++i) {
      for (int j = 0; j < size; ++j) {
        landscape[i][j].absorb();
        landscape[i][j].trickleAway(delta, *delta_locks);
      }
    }
  }
}
int normalTrickleIn(const int &start, const int &end, const int &size,
                    std::vector<std::vector<Point_pt>> &landscape,
                    std::vector<std::vector<float>> &delta) {
  int count = 0;
  for (int i = start; i < end; ++i) {
    for (int j = 0; j < size; ++j) {
      landscape[i][j].trickleIn(delta[i][j]);
      delta[i][j] = 0;
      if (landscape[i][j].isClean())
        ++count;
    }
  }
  return count;
}
int countAllPoints(const int &size,
                   std::vector<std::vector<Point_pt>> &landscape) {
  int count = 0;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if (landscape[i][j].isClean()) {
        count++;
      }
    }
  }
  return count;
}
void *runOneTimestamp(void *arg) {
  struct thread_info *info = (struct thread_info *)arg;
  int &start = info->start;
  int &end = info->end;
  int &size = info->size;
  int &raindropInterval = info->raindropInterval;
  pthread_barrier_t *barrier = info->barrier;
  std::vector<std::vector<float>> &delta = *info->delta;
  std::vector<std::vector<Point_pt>> &landscape = *info->landscape;
  std::vector<std::vector<pthread_mutex_t>> *delta_locks = info->delta_locks;
  int num_row = end - start;
  int *operationTime = new int;
  *operationTime = 0;
  bool waitingForOtherThreads = false;
  while (true) {
    ++*operationTime;
    normal_rainAndAbosrbAndTirckleAway(raindropInterval, start, end, size,
                                       landscape, delta, delta_locks);
    pthread_barrier_wait(barrier);
    if (waitingForOtherThreads) {
      int count;
      count = countAllPoints(size, landscape);
      pthread_barrier_wait(barrier);
      if (count == size * size) {
        break;
      }
    } else {
      int count;
      count = normalTrickleIn(start, end, size, landscape, delta);
      pthread_barrier_wait(barrier);
      if (count == num_row * size) {
        waitingForOtherThreads = true;
        count = countAllPoints(size, landscape);
        if (count == size * size) {
          break;
        }
      }
    }
  }
  delete (struct thread_info *)arg;
  pthread_exit(operationTime);
}

void RainfallSimulator_pt::runWholeProcess(
    std::vector<std::vector<pthread_mutex_t>> &mutex,
    pthread_barrier_t &barrier) {
  pthread_t *thrds = new pthread_t[num_thread];
  std::vector<std::vector<float>> delta(size, std::vector<float>(size, 0));
  int span = size / num_thread;
  auto start = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < num_thread; ++i) {
    struct thread_info *info = new struct thread_info;
    info->start = i * span;
    info->end = info->start + span;
    info->size = size;
    info->raindropInterval = raindropInterval;
    info->delta = &delta;
    info->landscape = &landscape;
    info->barrier = &barrier;
    info->delta_locks = &mutex;
    pthread_create(&thrds[i], NULL, &runOneTimestamp, (void *)info);
  }

  int max_operationTime = 0;
  for (int i = 0; i < num_thread; ++i) {
    void *operationTime = NULL;
    pthread_join(thrds[i], &operationTime);
    max_operationTime = std::max(*(int *)operationTime, max_operationTime);
    delete (int *)operationTime;
  }
    auto end = std::chrono::high_resolution_clock::now();
  delete[] thrds;
  fprintf(stdout, "Rainfall simulation took %d time steps to complete.\n",
          max_operationTime);
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  std::cout << "Runtime = " << duration.count() / 1e6 << " seconds\n";
  printAbsorbedRainDrops();
}

void RainfallSimulator_pt::printAbsorbedRainDrops() {
  for (auto points : landscape) {
    for (auto point : points) {
      std::cout.width(8);
      std::cout << point.reportAbsorbedDrops();
    }
    std::cout << std::endl;
  }
}

void RainfallSimulator_pt::printCurrentRainDrops() {
  for (auto points : landscape) {
    for (auto point : points) {
      std::cout.width(8);
      std::cout << point.reportCurrentDrops();
    }
    std::cout << std::endl;
  }
}
