#pragma once

#include "tetris.hpp"
#include "view.hpp"
#include <chrono>
#include <functional>
#include <vector>

template <typename View, typename Control>
void play_tetris(tetris& t, View&& view, Control&& control, std::mt19937& engine, unsigned int milliseconds)
{
  using namespace std;
  auto wait = [&](unsigned int ms)
    {
      const chrono::system_clock::time_point start = chrono::system_clock::now();
      auto elapsed_time = [&] {
        return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count();
      };
      // we want to refresh from time to time during waiting to avoid: "application not responding do you want to force quit?"
      // but not too much refresh for ascii view
      int k = 0;
      while (elapsed_time() < ms) {        
        if (k * 100 < elapsed_time()) {
          k++;
          view.draw(t);
        }        
      }
    };
  view.draw(t);
  wait(milliseconds);
  int cpt = 0;
  while (true)
    {
      for (auto a : control(t))
        {
          t.do_action(a);
          view.draw(t);
          wait(milliseconds);
        }
      // control(t) returns a sequence terminated by DROP, so no need to do: t.move_current_piece_down();
      int nb_lines = t.delete_possible_lines();
      t.add_to_lines(nb_lines);
      t.add_to_score(nb_lines * nb_lines);// * (t.get_level() + 1) * 10);
      // level is not usefull, because the AI is not affected by speed 
      // t.set_level(t.get_lines() / 10);
      if (t.is_game_over())
        {
          view.draw(t);
          break;
        }
      t.set_current_piece(t.get_next_piece());
      t.set_next_piece_at_random(engine);
      view.draw(t);
      wait(milliseconds);
    }
}

struct heuristic_control
{
  /*
     0:  height col 0
     ...
     9: height col 9
    10: max height
    11: delta col 0/1
    ...
    19: delta col 8/9
    20: max delta
    21: # holes col 0
    ...
    30: # holes col 9
    31: max holes
    32: # one square totally surrounded
    33: # deleted lines
   */
  static constexpr const int NWEIGHTS = 34;
  const std::vector<double>& weights;
  std::array<int, BOARD_WIDTH> height_cols;
  heuristic_control(const std::vector<double>& weights);
  double evaluate_position(tetris& t, int nb_lines);
  std::vector<tetris::action> operator()(const tetris& t);
  int height_col(const tetris& t, int j) const;
  int hole_col(const tetris& t, int j, int top) const;
  int surrounded(const tetris& t, int j, int top) const;
  static bool apply_seq(tetris& t, const std::vector<tetris::action>& seq, int& nb_lines);
};

int evaluate_control(const std::vector<double>& weights, int seed, int nb_games, int max_nb_iterations);

