# Tetris Game made for C++ course

## How to play:
- Move the falling blocks
- Complete horizontal lines to score points. Lines disappear once completed.
- The game speeds up as you progress to higher levels.

## Scoring System:
- 1 line cleared: 40 points
- 2 lines cleared: 100 points
- 3 lines cleared: 300 points
- 4 lines cleared (Tetris): 1200 points
- Bonus points are awarded based on the level.

## Controls:
- **Move Left:** A
- **Move Right:** D
- **Rotate Left:** J
- **Rotate Right:** K
- **Move Down:** S
- **Hard Drop:** Space
- **New Game:** <Any Key>

Play in browser:

[Tetris](https://dondejvo.github.io/Tetris/)

This was compiled by emscripten

## How to run

Prerequisities:

SDL2, SDL2_ttf and SDL2_image

Ubuntu:

```bash
sudo apt update
sudo apt install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev
```

Then build with cmake.

Targets:

- Tetris
- TetrisTests
