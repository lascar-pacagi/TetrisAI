#include "cem.hpp"
#include <functional>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <execution>

using namespace std;

cem::cem(const vector<double>& weights, int nb_iterations, int population_size,
         double elite_fraction, double stddev, double extra_stddev, 
         function<int(const vector<double>&, int)> fitness)
  : optimized_weights(weights), nb_iterations(nb_iterations),
    population_size(population_size), stddev(stddev), extra_stddev(extra_stddev), fitness(fitness)
{
  elite_size = population_size * elite_fraction;
}

void cem::run()
{
  const int WEIGHTS_SIZE = optimized_weights.size();
  vector<double> weights_mean = optimized_weights;
  vector<double> weights_std = vector<double>(WEIGHTS_SIZE, stddev);
  vector<vector<double>> population(population_size, vector<double>(WEIGHTS_SIZE));
  vector<pair<double, int>> fitness_index(population_size, {-1, 0});  
  vector<normal_distribution<double>> distributions(WEIGHTS_SIZE);
  int best_fitness = -1;
  mt19937 generator(chrono::system_clock::now().time_since_epoch().count());

  for (int iter = 0; iter < nb_iterations; ++iter)
    {
      for (int i = 0; i < WEIGHTS_SIZE; ++i)
        {
          distributions[i] = normal_distribution<double>(weights_mean[i],
                                                         weights_std[i]
                                                         + extra_stddev
                                                         * max(0.0, 1.0 - static_cast<double>(iter) / (static_cast<double>(nb_iterations) / 2)));
        }
      for (int i = 0; i < population_size; ++i)      
        {
          fitness_index[i].second = i;
          for (int j = 0; j < WEIGHTS_SIZE; ++j)
            {
              population[i][j] = distributions[j](generator);
            }
        }
      
      for_each(execution::par_unseq, begin(fitness_index), end(fitness_index), [&](pair<double, int>& p) {
        int i = distance(&fitness_index.data()[0], &p);
        p.first = fitness(population[i], iter);
      });
        
      sort(begin(fitness_index), end(fitness_index), greater<pair<double, int>>());
      
      cout << "generation: " << iter << endl;
      if (fitness_index[0].first > best_fitness)
        {
          best_fitness = fitness_index[0].first;
          optimized_weights = population[fitness_index[0].second];
          cout << "best fitness: " << best_fitness << endl;
        }

      for (int i = 0; i < WEIGHTS_SIZE; ++i)
        {
          weights_mean[i] = 0;
          weights_std[i] = 0;
        }

      // MEAN
      for (int i = 0; i < elite_size; ++i)
        {
          for (int j = 0; j < WEIGHTS_SIZE; ++j)
            {
              weights_mean[j] += population[fitness_index[i].second][j];
            }
        }

      for (int i = 0; i < WEIGHTS_SIZE; ++i)
        {
          weights_mean[i] /= elite_size;
        }

      // STDEV
      for (int i = 0; i < elite_size; ++i)
        {
          for (int j = 0; j < WEIGHTS_SIZE; ++j)
            {
              float v = population[fitness_index[i].second][j] - weights_mean[j];
              weights_std[j] += v * v;
            }
        }

      for (int i = 0; i < WEIGHTS_SIZE; ++i)
        {
          weights_std[i] = sqrt(weights_std[i] / elite_size);
        }
    }
    cout << "best fitness: " << best_fitness << endl;
}

vector<double> cem::get_best_weights() const
{
  return optimized_weights;
}
