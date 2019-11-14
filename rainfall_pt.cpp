#include "RainfallSimulator_pt.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <string>
#include <vector>

void readElevations(std::vector<std::vector<int>> &elevations,
                    const std::string &filename);
int main(int argc, char **argv) {
  if (argc != 6) {
    std::cerr << "./rainfall <num_threads> <interval_rainfall> "
                 "<rate_absorption> <dimension_landscape> <elevation_file>\n";
    exit(EXIT_FAILURE);
  }

  int P = atoi(argv[1]), M = atoi(argv[2]), N = atoi(argv[4]);
  float A = atof(argv[3]);
  std::string filename = argv[5];

  std::vector<std::vector<int>> elevations(N, std::vector<int>(N));
  readElevations(elevations, filename);

  RainfallSimulator_pt rainfallSimulator(elevations, P, N, M, A);
  auto start = std::chrono::high_resolution_clock::now();
  while (!rainfallSimulator.isComplete()) {
    rainfallSimulator.runOneTimestamp();
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  rainfallSimulator.printOperationTime();
  std::cout << "Runtime = " << duration.count() / 1e6 << " seconds\n";
  rainfallSimulator.printAbsorbedRainDrops();
}

void readElevations(std::vector<std::vector<int>> &elevations,
                    const std::string &filename) {
  int N = elevations.size();
  std::string line;
  std::ifstream ifs;
  ifs.open(filename);
  for (int i = 0; i < N; ++i) {
    getline(ifs, line);
    std::istringstream iss(line);
    for (int j = 0; j < N; ++j) {
      iss >> elevations[i][j];
    }
  }
  ifs.close();
}
