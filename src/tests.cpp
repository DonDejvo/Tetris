#include "catch.hpp"
#include "game.hpp"
#include <iostream>

namespace {
    bool compare_grids(const Game& game, int grid[ROWS][COLS]) {
        for(int i = 0; i < ROWS; ++i) {
            for(int j = 0; j < COLS; ++j) {
                int v = game.get_grid_item(j, i);
                if((grid[i][j] == 0 && v != 0) || (grid[i][j] != 0 && v == 0)) {
                    game.draw();
                    return false;
                }
            }
        }
        return true;
    }

    void init_game(Game& game) {
        game.set_next_figure_cb([]() { 
            static int idx = 0;
            int data[] { 2, 5, 0 };
            int next_figure = data[idx];
            idx = (idx + 1) % 3;
            return next_figure;
        });
        game.init();
    }
}

TEST_CASE("Basics: easy insertion", "[stage1]")
{

    SECTION("Empty trie")
    {
        Game g;
        init_game(g);
        g.translate_figure(0, 0, false, true);
        g.figure_next_tick();

        int expected[ROWS][COLS] {0};
        expected[18][4] = 1;
        expected[19][3] = 1;
        expected[19][4] = 1;
        expected[19][5] = 1;

        REQUIRE(compare_grids(g, expected));
    }

    SECTION("Rotation")
    {
        Game g;
        init_game(g);
        g.translate_figure(0, 1, false, true);
        g.figure_next_tick();

        int expected[ROWS][COLS] {0};
        expected[17][4] = 1;
        expected[18][4] = 1;
        expected[18][5] = 1;
        expected[19][4] = 1;

        REQUIRE(compare_grids(g, expected));
    }
}