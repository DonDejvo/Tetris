#include "animation.hpp"
#include <SDL_rect.h>
#include <SDL_render.h>

#include "game.hpp"

void Animation::start() {
  cur_frame = 0;
  counter = 0;
  is_first = true;
  playing = true;
}

void Animation::update(Uint32 dt) {
  if (!playing) {
    return;
  }
  bool is_next = false;
  if (counter >= frame_rate) {
    counter = 0;
    if (++cur_frame >= frame_count) {
      playing = false;
      end();
    } else {
      is_next = true;
    }
  }
  if (is_first || is_next) {
    is_first = false;
    next(cur_frame);
  }
  counter += dt;
}

void Animation::reset()
{
  cur_frame = 0;
  counter = 0;
}

void RowClearAnimation::next(int frame) {
  for (int i = 0; i < row_count; ++i) {
    int num = rows[i];
    game.set_grid_item(4 - frame, num, HIDDEN_TILE);
    game.set_grid_item(5 + frame, num, HIDDEN_TILE);
  }
}

void RowClearAnimation::end() {
  row_count = 0;
  game.clear_rows();
}

void RowClearAnimation::init_rows() {
  row_count = 0;
  for (int i = ROWS - 1; i >= 0; --i) {
    int tile_count = 0;
    for (int j = 0; j < COLS; ++j) {
      if (game.get_grid_item(j, i)) {
        ++tile_count;
      }
    }
    if (tile_count == COLS) {
      rows[row_count++] = i;
    }
  }
}

void GameOverAnimation::next(int frame) {}

void GameOverAnimation::end() { game.set_gameover(); }

void GameOverAnimation::draw_bg() {
  game.draw_gameover_stripes(cur_frame);
}