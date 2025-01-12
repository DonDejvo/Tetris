#include "catch.hpp"
#include "game.hpp"
#include <iostream>

namespace {
    bool compare_grids(const Game& game, int grid[ROWS][COLS]) {
        for(int i = 0; i < ROWS; ++i) {
            for(int j = 0; j < COLS; ++j) {
                int v = game.get_grid_item(j, i);
                if((grid[i][j] == 0 && v != 0) || (grid[i][j] != 0 && v == 0)) {
                    game.print_grid();
                    return false;
                }
            }
        }
        return true;
    }
}

TEST_CASE("Movement", "[stage1]")
{

    SECTION("Translation")
    {
        Game g;
        g.set_next_figure_cb([]() { 
            static int i = 0;
            int data[] { 2, 5 };
            return data[i++ % 2]; });
        g.init();
        g.translate_figure(1);
        g.drop_figure();
        g.figure_next_tick();

        int expected[ROWS][COLS] {0};
        expected[18][4] = 1;
        expected[18][5] = 1;
        expected[18][6] = 1;
        expected[19][5] = 1;

        REQUIRE(compare_grids(g, expected));

        g.translate_figure(-5);
        g.drop_figure();
        g.figure_next_tick();

        expected[18][0] = 1;
        expected[18][1] = 1;
        expected[18][2] = 1;
        expected[19][2] = 1;

        REQUIRE(compare_grids(g, expected));

        g.destroy();
    }

    SECTION("Rotation")
    {
        Game g;
        g.set_next_figure_cb([]() { 
            static int i = 0;
            int data[] { 2, 5 };
            return data[i++ % 2]; });
        g.init();
        g.rotate_figure(RotDir::RIGHT);
        g.drop_figure();
        g.figure_next_tick();

        int expected[ROWS][COLS] {0};
        expected[17][4] = 1;
        expected[18][4] = 1;
        expected[18][3] = 1;
        expected[19][4] = 1;

        REQUIRE(compare_grids(g, expected));

        g.rotate_figure(RotDir::LEFT);
        g.rotate_figure(RotDir::LEFT);
        g.rotate_figure(RotDir::RIGHT);
        g.rotate_figure(RotDir::LEFT);
        g.drop_figure();
        g.figure_next_tick();

        expected[15][3] = 1;
        expected[16][3] = 1;
        expected[16][4] = 1;
        expected[16][5] = 1;

        REQUIRE(compare_grids(g, expected));

        g.destroy();
    }
}