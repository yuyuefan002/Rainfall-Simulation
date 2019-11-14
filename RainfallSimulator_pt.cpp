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
    landscape.push_back(std::vector<Point>());
    for (int j = 0; j < size; ++j) {
      std::vector<std::pair<int, int>> lowerPoints =
          checkLowestNeighbor(elevations, i, j);
      landscape[i].push_back(Point(A, lowerPoints));
    }
  }
}

void RainfallSimulator_pt::runOneTimestamp() {
  std::vector<std::vector<float>> delta(size, std::vector<float>(size, 0));
  ++operationTime;

  if (raindropInterval != 0) {
    --raindropInterval;
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        landscape[i][j].rainfall();
        landscape[i][j].absorb();
        landscape[i][j].trickleAway(delta);
      }
    }
  } else {
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        landscape[i][j].absorb();
        landscape[i][j].trickleAway(delta);
      }
    }
  }

  int count = 0;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      landscape[i][j].trickleIn(delta[i][j]);
      if (landscape[i][j].isClean())
        ++count;
    }
  }

  if (count == size * size) {
    complete = true;
  }
}

bool RainfallSimulator_pt::isComplete() { return complete; }

void RainfallSimulator_pt::printOperationTime() {
  fprintf(stdout, "Rainfall simulation took %d time steps to complete.\n",
          operationTime);
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