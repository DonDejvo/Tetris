#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <SDL2/SDL.h>

class Game;

class Animation {
protected:
  int cur_frame, counter, frame_rate, frame_count;
  bool is_first, playing;
  Game& game;

public:
  Animation(Game& game, int frame_rate, int frame_count)
      : frame_rate(frame_rate), frame_count(frame_count) , game(game) {
    playing = false;
    is_first = true;
  }
  inline bool is_playing() const { return playing; }
  void start();
  void update(Uint32 dt);
  virtual void next(int frame) = 0;
  virtual void end() = 0;
  void reset();
};

class RowClearAnimation : public Animation {
private:
  int rows[10];
  int row_count;

public:
  RowClearAnimation(Game& game) : Animation(game, 80, 5) {}
  inline int get_row_count() const { return row_count; }
  void next(int frame) override;
  void end() override;
  void init_rows();
};

class GameOverAnimation : public Animation {
public:
  GameOverAnimation(Game& game) : Animation(game, 80, 16) {}
  void next(int frame) override;
  void end() override;
  void draw_bg();
};

#endif