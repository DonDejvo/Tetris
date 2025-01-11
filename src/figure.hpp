#ifndef FIGURE_H
#define FIGURE_H

#include "window.hpp"

#define FIGURE_COUNT 7

const int figures[FIGURE_COUNT][7]{
    0, 1, 2, 3, 4, 2, 2, // I
    1, 2, 5, 6, 4, 2, 1, // O
    0, 1, 2, 5, 3, 1, 2, // T
    0, 1, 5, 6, 3, 3, 3, // Z
    1, 2, 4, 5, 3, 3, 0, // S
    0, 1, 2, 6, 3, 1, 0, // J
    0, 1, 2, 4, 3, 1, 3, // L
};

// const std::string level_colors[LEVEL_COLOR_COUNT][2]{
//     {"\033[94m", "\033[34m"}, // lightblue, blue
//     {"\033[33m", "\033[32m"}, // yellow, green
//     {"\033[95m", "\033[35m"}, // violet, purple
//     {"\033[32m", "\033[34m"}, // green, blue
//     {"\033[36m", "\033[35m"}, // cyan, purple
//     {"\033[34m", "\033[36m"}, // blue, cyan
//     {"\033[31m", "\033[34m"} // red, blue
// };

struct Point
{
  int x, y;
};

class Figure
{
private:
  int n;
  Point position, offset;
  int rotation;
  int texture_id;
  Point points[4];

public:
  void init(int n);
  void move(int dx, int dy);
  void rotate(int dir);
  bool is_gameover();
  inline int get_id() const { return n; };
  inline Point get_position() const { return position; };
  Point get_point(int i) const;
  inline int get_texture_id() const { return texture_id; };
  static Point get_figure_point(int figure_id, int idx);
  // static std::string get_draw_color(int tex_id, int level);
};

#endif