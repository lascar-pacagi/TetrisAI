#pragma once
#include <functional>
#include <random>

class cem
{
  std::vector<double> optimized_weights;
  int nb_iterations;
  int population_size;
  double stddev;
  double extra_stddev;
  std::function<int(const std::vector<double>&, int)> fitness;
  int elite_size;

public:
  cem(const std::vector<double>& weights, int nb_iterations, int population_size, double elite_fraction, 
      double stddev, double extra_stddev, std::function<int(const std::vector<double>&, int)> fitness);
  void run();
  std::vector<double> get_best_weights() const;
};
