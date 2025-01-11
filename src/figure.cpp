#include "figure.hpp"

void Figure::init(int n)
{
    this->n = n;
    position.x = 0;
    position.y = 0;
    rotation = 0;
    offset.x = figures[n][4];
    offset.y = figures[n][5];
    for (int i = 0; i < 4; ++i)
    {
        points[i] = get_figure_point(n, i);
    }
    texture_id = figures[n][6];

    move(3, 0);
}

void Figure::move(int dx, int dy)
{
    position.x += dx;
    position.y += dy;
    for (int i = 0; i < 4; ++i)
    {
        points[i].x += dx;
        points[i].y += dy;
    }
}

void Figure::rotate(int dir)
{
    if(n == 1) return;

    dir = dir < 0 ? -1 : 1;

    if (n == 0 || n == 3 || n == 4)
    {
        if((rotation == 1 && dir == 1) || (rotation == 0 && dir == -1)) {
            dir *= -1;
        }
    }

    rotation = (rotation + dir) % 4;

    for (int i = 0; i < 4; ++i)
    {
        int x = ((points[i].x - position.x) * 2 + 1 - offset.x);
        int y = ((points[i].y - position.y) * 2 + 1 - offset.y);
        points[i].x = (dir * -y + offset.x - 1) / 2 + position.x;
        points[i].y = (dir * x + offset.y - 1) / 2 + position.y;
    }
}

bool Figure::is_gameover()
{
    for (int i = 0; i < 4; ++i)
    {
        Point p = points[i];
        if (p.y == 0)
            return true;
    }
    return false;
}

Point Figure::get_point(int i) const
{
    return points[i];
}

Point Figure::get_figure_point(int figure_id, int idx)
{
    return Point{figures[figure_id][idx] % 4, figures[figure_id][idx] / 4};
}

// std::string Figure::get_draw_color(int tex_id, int level)
// {
//     return level_colors[(level) % LEVEL_COLOR_COUNT][tex_id % 2];
// }
