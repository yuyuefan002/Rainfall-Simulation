#include "Point_pt.h"
#include "RainfallSimulator_pt.h"
#include <chrono>
#include <fstream>
#include <pthread.h>
#include <sstream>

extern pthread_barrier_t barrier;
void readElevations(std::vector<std::vector<int>> &elevations,
                    const std::string &filename);

int main(int argc, char *argv[]) {
  if (argc != 6) {
    std::cerr << "./rainfall <num_threads> <interval_rainfall> "
                 "<rate_absorption> <dimension_landscape> <elevation_file>\n";
    exit(EXIT_FAILURE);
  }

  int num_thread = atoi(argv[1]), raindropInterval = atoi(argv[2]),
      size = atoi(argv[4]);
  float absorptionRate = atof(argv[3]);
  std::string filename = argv[5];

  if (num_thread > size) {
    num_thread = size;
  }

  Landscape<Point_pt> landscape(size + 2);

  std::vector<std::vector<int>> elevations(size, std::vector<int>(size));
  readElevations(elevations, filename);
  initLandscape(landscape, size, elevations, absorptionRate);

  pthread_mutex_t *mutex_array = new pthread_mutex_t[(size + 1) * (size + 1)];

  for (int i = 1; i < size + 1; ++i) {
    for (int j = 1; j < size + 1; ++j)
      pthread_mutex_init(&mutex_array[i * size + j], NULL);
  }
  pthread_barrier_init(&barrier, NULL, num_thread);

  pthread_t *thrds = new pthread_t[num_thread];
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < num_thread; ++i) {
    struct thread_info *info = new thread_info;
    info->num_thread = num_thread;
    info->raindropInterval = raindropInterval;
    info->size = size;
    info->landscape = &landscape;
    info->mutex_array = mutex_array;
    info->thread_id = i;
    pthread_create(&thrds[i], NULL, rainfall, (void *)(info));
  }

  for (int i = 0; i < num_thread; ++i) {
    pthread_join(thrds[i], NULL);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::microseconds>(end - start);

  printResult(duration.count(), landscape, size);

  delete[] mutex_array;
  delete[] thrds;
  return EXIT_SUCCESS;
}

void readElevations(std::vector<std::vector<int>> &elevations,
                    const std::string &filename) {
  int size = elevations.size();
  std::string line;
  std::ifstream ifs;
  ifs.open(filename);
  for (int i = 0; i < size; ++i) {
    getline(ifs, line);
    std::istringstream iss(line);
    for (int j = 0; j < size; ++j) {
      iss >> elevations[i][j];
    }
  }
  ifs.close();
}
