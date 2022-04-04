#include "control.hpp"
using namespace std;

heuristic_control::heuristic_control(const vector<double>& weights)
  : weights(weights)
{
}

static const vector<vector<tetris::action>> sequence[NB_KINDS] =
  {
    { // O_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // I_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP},

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // S_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP},

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // Z_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP},

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // L_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // J_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // T_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
  };

int heuristic_control::height_col(const tetris& t, int j) const
{
  for (int i = 0; i < BOARD_HEIGHT; ++i)
    {
      if (t.get(i, j) != FREE) return BOARD_HEIGHT - i;
    }
  return 0;
}

int heuristic_control::hole_col(const tetris& t, int j, int top) const
{
  int res = 0;
  for (int i = top + 1; i < BOARD_HEIGHT; ++i)
    {
      if (t.get(i, j) == FREE) ++res;
    }
  return res;
}

int heuristic_control::surrounded(const tetris& t, int j, int top) const
{
  static int di[] = { 0, 0, -1, 1 };
  static int dj[] = { -1, 1, 0, 0 };
  int res = 0;
  for (int i = top + 1; i < BOARD_HEIGHT; ++i)
    {
      if (t.get(i, j) == FREE)
        {
          bool trapped = true;
          for (int k = 0; k < 4; ++k)
            {
              int ii = i + di[k];
              int jj = j + dj[k];
              if (ii >= 0 && ii < BOARD_HEIGHT && jj >= 0 && jj < BOARD_WIDTH && t.get(ii, jj) == FREE)
                {
                  trapped = false;
                  break;
                }
            }
          res += trapped;
        }
    }
  return res;
}

double heuristic_control::evaluate_position(tetris& t, int nb_lines)
{
  double value = nb_lines * weights[33];
  int max_height = 0;
  for (int i = 0; i < BOARD_WIDTH; ++i)
    {
      height_cols[i] = height_col(t, i);
      value += weights[i] * height_cols[i];
      max_height = max(max_height, height_cols[i]);
    }
  value += weights[10] * max_height;
  int max_delta = 0;
  for (int i = 0; i < BOARD_WIDTH - 1; ++i)
    {
      int d = abs(height_cols[i + 1] - height_cols[i]);
      value += d * weights[11 + i];
      max_delta = max(max_delta, d);
    }
  value += weights[20] * max_delta;
  int max_hole = 0;
  for (int i = 0; i < BOARD_WIDTH; ++i)
    {
      int h = hole_col(t, i, BOARD_HEIGHT - height_cols[i]);
      max_hole = max(max_hole, h);
      value += weights[21 + i] * h;
    }
  value += weights[31] * max_hole;
  int totally_surrounded = 0;
  for (int i = 0; i < BOARD_WIDTH; ++i)
    {
      totally_surrounded += surrounded(t, i, BOARD_HEIGHT - height_cols[i]);
    }
  value += weights[32] * totally_surrounded;
  return value;
}

bool heuristic_control::apply_seq(tetris& t, const vector<tetris::action>& seq, int& nb_lines)
{
  nb_lines = 0;
  for (uint8_t a : seq)
    {
      if (!t.do_action(tetris::action(a))) return false;
    }
  t.drop_current_piece();
  nb_lines = t.count_possible_lines();
  return true;
}

vector<tetris::action> heuristic_control::operator()(const tetris& t)
{
  vector<tetris::action> res = { tetris::DROP };
  double best_value = numeric_limits<double>::lowest();
  int current_piece_kind = t.get_current_piece().get_kind();
  int next_piece_kind = t.get_next_piece().get_kind();
  for (const auto& seq1 : sequence[current_piece_kind])
    {
      int nb_lines_seq1 = 0;
      tetris t1 = t;
      if (!apply_seq(t1, seq1, nb_lines_seq1)) continue;
      if (t1.is_game_over())
        {
          double v = evaluate_position(t1, nb_lines_seq1);
          if (v > best_value)
            {
              best_value = v;
              res = seq1;
            }
          continue;
        }
      t1.set_current_piece(t1.get_next_piece());
      for (const auto& seq2 : sequence[next_piece_kind])
        {
          tetris t2 = t1;
          int nb_lines_seq2;
          if (apply_seq(t2, seq2, nb_lines_seq2))
            {
              double v = evaluate_position(t2, nb_lines_seq1 + nb_lines_seq2);
              if (v > best_value)
                {
                  best_value = v;
                  res = seq1;
                }
            }
        }
    }
  return res;
}

int evaluate_control(const vector<double>& weights, int seed, int nb_games, int max_nb_iterations)
{
  mt19937 engine(seed);
  heuristic_control control(weights);
  int res = 0;
  for (int i = 0; i < nb_games; ++i)
    {
      tetris t(engine);
      int nb_iterations = 0;
      while (nb_iterations < max_nb_iterations)
        {
          ++nb_iterations;
          for (auto a : control(t))
            {
              t.do_action(a);
            }
          // control(t) returns a sequence terminated by DROP, so no need to do: t.move_current_piece_down();
          int nb_lines = t.delete_possible_lines();
          t.add_to_lines(nb_lines);
          t.add_to_score(nb_lines * nb_lines);
          if (t.is_game_over()) break;
          t.set_current_piece(t.get_next_piece());
          t.set_next_piece_at_random(engine);            
        }
      res += t.get_score() + nb_iterations;
    }
  return res;
}
