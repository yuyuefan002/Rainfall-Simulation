#include "RainfallSimulator_pt.h"
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
  if (P > N)
    P = N;
  std::string filename = argv[5];

  std::vector<std::vector<int>> elevations(N, std::vector<int>(N));
  readElevations(elevations, filename);

  RainfallSimulator_pt rainfallSimulator(elevations, P, N, M, A);

  std::vector<std::vector<pthread_mutex_t>> mutex(
      N, std::vector<pthread_mutex_t>(N));

  pthread_barrier_t barrier;
  pthread_barrier_init(&barrier, NULL, P);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      pthread_mutex_init(&mutex[i][j], NULL);
    }
  }
  rainfallSimulator.runWholeProcess(mutex, barrier);
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
