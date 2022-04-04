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
#include "argparse.h"

using namespace std;
using namespace argparse;

int main(int argc, const char *argv[])
{
  ios_base::sync_with_stdio(false);
  ArgumentParser parser("play", "play tetris with learnt weights");
  parser.add_argument("-w", "--weights", "filename of learnt weights", true);
  parser.add_argument("-d", "--display", "no/ascii/graphic", false);
  parser.add_argument("-t", "--time", "time for one step in game in milliseconds", false);
  parser.enable_help();
  auto err = parser.parse(argc, argv);
  if (err) {
    cout << err << endl;
    return -1;
  }
  if (parser.exists("help")) {
    parser.print_help();
    return 0;
  }
  ifstream ifs(parser.get<string>("w"), ifstream::in);
  vector<double> weights;
  for (double w; ifs >> w;) {
    weights.push_back(w);
  }
  heuristic_control control(weights);
  unsigned int ms = 50;
  if (parser.exists("t")) {
    ms = parser.get<unsigned int>("t");
  }
  mt19937 engine(chrono::system_clock::now().time_since_epoch().count());
  tetris game(engine);
  auto play = [&]<typename View>(View&& view) {
    play_tetris(game, forward<View>(view), control, engine, ms);
  };
  if (parser.exists("d")) {
    string display = parser.get<string>("d");
    if (display == "no") play(no_view{});
    else if (display == "ascii") play(ascii_view{});
    else play(graphical_view{});
  } else {
    play(graphical_view{});
  }
  cout << "score: " << game.get_score() << '\n'
       << "lines: " << game.get_lines() << endl;
}
