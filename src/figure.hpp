#ifndef FIGURE_H
#define FIGURE_H

#include "window.hpp"

enum RotDir {
  LEFT = -1, RIGHT = 1, NONE = 0
};

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
  void rotate(RotDir dir);
  bool is_gameover();
  inline int get_id() const { return n; };
  inline Point get_position() const { return position; };
  Point get_point(int i) const;
  inline int get_texture_id() const { return texture_id; };
  static Point get_figure_point(int figure_id, int idx);
};

#endif