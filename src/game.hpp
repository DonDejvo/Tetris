#ifndef GAME_H
#define GAME_H

#include <condition_variable>
#include <memory>
#include "window.hpp"
#include "figure.hpp"
#include <functional>
#include "animation.hpp"

#define GAME_WIDTH 1152
#define GAME_HEIGHT 960
#define GRID_X 420
#define GRID_Y 160
#define SCORE_X 880
#define SCORE_Y 200
#define NEXT_FIGURE_X 880
#define NEXT_FIGURE_Y 430
#define LEVEL_X 880
#define LEVEL_Y 690
#define LINES_X 420
#define LINES_Y 40
#define STATISTICS_X 60
#define STATISTICS_Y 260
#define ROWS 20
#define COLS 10
#define PERIOD 50
#define FPS 20
#define HIDDEN_TILE 69
#define LEVEL_COLOR_COUNT 10

const Uint32 level_colors[LEVEL_COLOR_COUNT][2]{
  0x2538e6ff, 0x63bcf9ff,
  0x4ca730ff, 0x97d143ff,
  0xad24b7ff,0xe780f8ff,
  0x2538e6ff, 0x79dc60ff,
  0xd42d5cff, 0x8bf8a5ff,
  0x8bf8a5ff, 0x6895f8ff,
  0xcc3b1fff, 0x737373ff,
  0x7817eeff, 0x9e1f1cff,
  0x2538e6ff, 0xcc3b1fff,
  0xcc3b1fff, 0xf19f4fff
};

struct GameConfig {
    int level;
};

class Game
{
public:
    static void run(const GameConfig& config);
    static void help();
    Game();
    Game(const GameConfig& config);
    void init();
    void destroy();

    bool is_running() const;
    void loop();
    void draw() const;
    void update(Uint32 dt);
    void output();
    void input();
    int get_grid_item(int x, int y) const;
    void set_grid_item(int x, int y, int v);
    bool check_collisions();
    void translate_figure(int dx, int rot_dir, bool down, bool drop);
    void figure_next_tick();
    void grid_insert();
    void draw_figure(int figure_id, int x, int y) const;
    void draw_figure() const;
    void draw_grid() const;
    void draw_sidebar() const;
    void draw_gameover_stripes(int count) const;
    void init_next_figure();
    void clear_rows();
    void set_next_figure_cb(const std::function<int()>& cb);
    inline void set_gameover() { gameover = true; top = std::max(score, top); }
    inline int get_width() const { return width; }
    inline int get_height() const { return height; }
    void start_game();
    int get_level_tex_offset() const;

private:
    const GameConfig& config;
    int width, height;
    std::function<int()> next_figure_cb = [](){ return rand() % FIGURE_COUNT; };
    Texture textures[256];
    bool running = false;
    Uint32 last_time, acc_time = 0;
    std::mutex mutex;
    std::condition_variable cvar;
    std::unique_ptr<Window> win;
    Figure figure;
    int grid[ROWS][COLS]{0};
    int cur_figure, next_figure;
    bool gameover = false;
    int top = 0, score, level, rows_cleared;
    int tick_couter = 0;
    bool down_pressed = false, can_move_down = true;
    std::unique_ptr<RowClearAnimation> row_clear_anim;
    std::unique_ptr<GameOverAnimation> game_over_anim;
    int statistics[FIGURE_COUNT] {0};
};

#endif
