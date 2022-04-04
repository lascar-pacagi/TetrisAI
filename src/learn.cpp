#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include <chrono>
#include <random>
#include <fstream>
#include "tetris.hpp"
#include "view.hpp"
#include "control.hpp"
#include "cem.hpp"
#include "argparse.h"

using namespace std;
using namespace argparse;

int main(int argc, const char *argv[])
{
    ios_base::sync_with_stdio(false);
    ArgumentParser parser("learn", "learn to play tetris with noisy cross entropy method");
    parser.add_argument("-w", "--weights", "filename to save weights", true);
    parser.add_argument("-n", "--nb-iterations", "number of generations (default: 80)", false);
    parser.add_argument("-p", "--population-size", "population size (default: 100)", false);
    parser.add_argument("-f", "--elite-fraction", "top ratio to create new generation (default: 0.3)", false);    
    parser.add_argument("-s", "--stddev", "standard deviation (default: 1.0)", false);
    parser.add_argument("-e", "--extra-stddev", "extra standard deviation (default: 20.0)", false);    
    parser.add_argument("-g", "--nb-games", "number of games to evaluate fitness (default: 2)", false);    
    parser.add_argument("-m", "--max-in-game", "maximum number of iterations in a game to evaluate fitness (default: 100000)", false);    
    parser.enable_help();
    auto err = parser.parse(argc, argv);
    if (err)
    {
        cout << err << endl;
        return -1;
    }
    if (parser.exists("help"))
    {
        parser.print_help();
        return 0;
    }
    ofstream ofs(parser.get<string>("w"), ofstream::out);    
    int nb_iterations = 80;
    int population_size = 100;
    float elite_fraction = 0.3;
    float stddev = 1.0;
    float extra_stddev = 20.0;
    int nb_games = 2;
    int max_nb_iterations_in_game = 100000;
    if (parser.exists("n")) 
    {
        nb_iterations = parser.get<int>("n");
    }
    if (parser.exists("p")) 
    {
        population_size = parser.get<int>("p");
    }
    if (parser.exists("f")) 
    {
        elite_fraction = parser.get<double>("f");
    }
    if (parser.exists("s")) 
    {
        stddev = parser.get<double>("s");
    }
    if (parser.exists("e")) 
    {
        extra_stddev = parser.get<double>("e");
    }
    if (parser.exists("g")) 
    {
        nb_games = parser.get<int>("g");
    }
    if (parser.exists("m")) 
    {
        max_nb_iterations_in_game = parser.get<int>("m");
    }
    auto fitness = [&](const std::vector<double>& weights, int seed) {
        return evaluate_control(weights, seed, nb_games, max_nb_iterations_in_game);
    };
    cem opti{
        vector<double>(heuristic_control::NWEIGHTS, 0),
        nb_iterations,
        population_size,
        elite_fraction,
        stddev,        
        extra_stddev,
        fitness
    };
    opti.run();
    for (double w : opti.get_best_weights()) {
        ofs << w << '\n';
    }
}
