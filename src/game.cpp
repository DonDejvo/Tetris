#include "game.hpp"
#include <thread>
#include <stdlib.h>
#include <mutex>
#include <iostream>
#include "argparser.hpp"
#include "utils.hpp"

void Game::run(const GameConfig &config)
{
    // auto computeThread = [](Game &g)
    // {
    //     while (g.is_running())
    //     {
    //         g.update();
    //     }
    // };
    // auto outputThread = [](Game &g)
    // {
    //     while (g.is_running())
    //     {
    //         g.output();
    //     }
    // };
    // auto inputThread = [](Game &g)
    // {
    //     while (g.is_running())
    //     {
    //         g.input();
    //     }
    // };

    // system("stty raw");

    Game g(config);
    g.init();

    while (g.is_running())
    {
        g.loop();
    }

    g.destroy();

    // std::thread t1(computeThread, std::ref(g));
    // std::thread t2(outputThread, std::ref(g));
    // std::thread t3(inputThread, std::ref(g));

    // t1.join();
    // t2.join();
    // t3.join();

    // system("stty -raw");
}

void Game::help()
{
    std::cout << "Tetris Game made for C++ course\n"
              << "How to play:\n"
              << "  - Move the falling blocks\n"
              << "  - Complete horizontal lines to score points. Lines disappear once completed.\n"
              << "  - The game speeds up as you progress to higher levels.\n"
              << "\n"
              << "Scoring System:\n"
              << "  - 1 line cleared: 40 points\n"
              << "  - 2 lines cleared: 100 points\n"
              << "  - 3 lines cleared: 300 points\n"
              << "  - 4 lines cleared (Tetris): 1200 points\n"
              << "  - Bonus points are awarded based on the level.\n"
              << "Controls:\n"
              << "  - Move Left:\t\t A\n"
              << "  - Move Right:\t\t D\n"
              << "  - Rotate Left:\t J\n"
              << "  - Rotate Right:\t K\n"
              << "  - Move Down:\t\t S\n"
              << "  - Hard Drop:\t\t Space\n"
              << "  - New Game:\t\t <Any Key>\n"
              << "Usage:\n"
              << "\tTetris [options]\n"
              << "Options:\n"
              << "\t--level <value>\t = Set the game starting level (default: " << ARG_LEVEL_DEFAULT << ", min: " << ARG_LEVEL_MIN << ", max: " << ARG_LEVEL_MAX << ").\n"
              << "\t-h,--help\t = Print usage information and exit.\n"
              << std::endl;
}

Game::Game() : Game({0})
{
}

bool Game::is_running() const
{
    return running;
}

void Game::loop()
{
    Uint32 cur_time = SDL_GetTicks();
    Uint32 dt = 1000 / FPS;
    acc_time += cur_time - last_time;
    last_time = cur_time;
    SDL_Delay(std::max(dt - (cur_time - last_time), (Uint32)0));
    update(dt);
    draw();
}

void Game::init()
{
    win->init();

    for (int i = 0; i < LEVEL_COLOR_COUNT; ++i)
    {
        Uint32 color1 = level_colors[i][0];
        Uint32 color2 = level_colors[i][1];
        int offset = i * 8;

        textures[offset + 0] = win->generate_texture(color1, 1);
        textures[offset + 1] = win->generate_texture(color1, 2);
        textures[offset + 2] = win->generate_texture(color1, 3);
        textures[offset + 3] = win->generate_texture(color2, 1);

        textures[offset + 5] = win->generate_texture(color2, 4);
        textures[offset + 6] = win->generate_texture(WHITE, 4);
        textures[offset + 7] = win->generate_texture(color1, 4);
        textures[offset + 8] = win->generate_texture(BLACK, 4);
    }

    textures[128] = win->load_image_texture("assets/background.gif");

    running = true;
    last_time = SDL_GetTicks();

    start_game();
}

void Game::destroy()
{
    for (int i = 0; i < 32; ++i)
    {
        textures[i].destroy();
    }
    win->destroy();
}

void Game::draw() const
{
    win->begin_frame();
    win->draw_image(textures[128], 0, 0, GAME_WIDTH, GAME_HEIGHT);
    draw_grid();
    draw_figure();
    draw_sidebar();
    if (gameover || game_over_anim->is_playing())
    {
        game_over_anim->draw_bg();
    }
    win->end_frame();
}

void Game::update(Uint32 dt)
{

    int dx = 0;
    RotDir rotation_dir = NONE;
    bool drop_pressed = false;

    ////////INPUT////////
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (gameover)
            {
                start_game();
                return;
            }
            else if (before_start_countdown > 0)
            {
                before_start_countdown = 0;
            }
            switch (e.key.keysym.sym)
            {
            case SDLK_a:
                dx = -1;
                break;
            case SDLK_d:
                dx = 1;
                break;
            case SDLK_k:
                rotation_dir = RotDir::RIGHT;
                break;
            case SDLK_j:
                rotation_dir = RotDir::LEFT;
                break;
            case SDLK_s:
                down_pressed = true;
                break;
            case SDLK_SPACE:
                drop_pressed = true;
                break;
            }
            break;
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
            case SDLK_s:
                down_pressed = false;
                can_move_down = true;
            }
            break;
        }
    }

    if (gameover)
        return;

    if (before_start_countdown > 0)
    {
        before_start_countdown -= dt;
        return;
    }

    if (game_over_anim->is_playing())
    {
        game_over_anim->update(dt);
        return;
    }

    if (row_clear_anim->is_playing())
    {
        row_clear_anim->update(dt);
        return;
    }

    translate_figure(dx);
    rotate_figure(rotation_dir);

    if (drop_pressed)
    {
        drop_figure();
    }
    else if (down_pressed && can_move_down)
    {
        move_down();
    }

    ////////TICK////////
    tick_couter += dt;
    if (tick_couter >= level_ticks[level < 10 ? level : 10])
    {
        tick_couter = 0;
        figure.move(0, 1);
        if (!check_collisions())
        {
            figure.move(0, -1);
            if (figure.is_gameover())
            {
                game_over_anim->start();
            }
            else
            {
                grid_insert();
                row_clear_anim->init_rows();
                if (row_clear_anim->get_row_count() > 0)
                {
                    row_clear_anim->start();
                }
                init_next_figure();
            }
        }
    }
}

int Game::get_grid_item(int x, int y) const
{
    return grid[y][x];
}

void Game::set_grid_item(int x, int y, int v)
{
    grid[y][x] = v;
}

/**
 * Returns false if figure collides
 */
bool Game::check_collisions()
{
    for (int i = 0; i < 4; ++i)
    {
        Point p = figure.get_point(i);
        if (p.x < 0 || p.x > COLS - 1 || p.y > ROWS - 1 || (p.y >= 0 && get_grid_item(p.x, p.y)))
        {
            return false;
        }
    }
    return true;
}

void Game::translate_figure(int dx)
{
    int n = dx > 0 ? 1 : -1;
    for (int i = 0; i < abs(dx); ++i)
    {
        figure.move(n, 0);
        if (!check_collisions())
        {
            figure.move(-n, 0);
        }
    }
}

void Game::rotate_figure(RotDir rot_dir)
{
    figure.rotate(rot_dir);
    if (!check_collisions())
    {
        figure.rotate(rot_dir == RotDir::LEFT ? RotDir::RIGHT : RotDir::LEFT);
    }
}

void Game::drop_figure()
{
    while (true)
    {
        figure.move(0, 1);
        if (!check_collisions())
        {
            figure.move(0, -1);
            break;
        }
    }
}

void Game::move_down()
{
    figure.move(0, 1);
    if (!check_collisions())
    {
        figure.move(0, -1);
    }
}

void Game::figure_next_tick()
{
    figure.move(0, 1);
    if (!check_collisions())
    {
        figure.move(0, -1);
        if (figure.is_gameover())
        {
            game_over_anim->start();
        }
        else
        {
            grid_insert();
            clear_rows();
            init_next_figure();
        }
    }
}

void Game::grid_insert()
{
    for (int i = 0; i < 4; ++i)
    {
        Point p = figure.get_point(i);
        set_grid_item(p.x, p.y, figure.get_texture_id() + 1);
    }
}

void Game::draw_figure(int figure_id, int x, int y) const
{
    for (int i = 0; i < 4; ++i)
    {
        Point p = Figure::get_figure_point(figure_id, i);
        win->draw_tile(x + p.x * TILE_SIZE, y + p.y * TILE_SIZE, textures[get_level_tex_offset() + figures[figure_id][6]]);
    }
}

void Game::draw_figure() const
{
    for (int i = 0; i < 4; ++i)
    {
        Point p = figure.get_point(i);
        if (p.y < 0)
            continue;
        win->draw_tile(GRID_X + p.x * TILE_SIZE, GRID_Y + p.y * TILE_SIZE, textures[get_level_tex_offset() + figure.get_texture_id()]);
    }
}

void Game::draw_grid() const
{
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
        {
            int tile = grid[i][j];
            if (tile && tile != HIDDEN_TILE)
            {
                int texture_id = tile - 1;
                win->draw_tile(GRID_X + TILE_SIZE * j, GRID_Y + TILE_SIZE * i, textures[get_level_tex_offset() + texture_id]);
            }
        }
    }
}

void Game::draw_sidebar() const
{
    win->draw_text("STATISTICS", STATISTICS_X + 20, STATISTICS_Y, 0xFFFFFFFF);
    for (int i = 0; i < FIGURE_COUNT; ++i)
    {
        int offset_y = i * 90 + 60;
        draw_figure(i, STATISTICS_X, STATISTICS_Y + offset_y);
        std::string str = std::to_string(statistics[i]);
        add_leading_zeros(3, str);
        win->draw_text(str, STATISTICS_X + 160, STATISTICS_Y + offset_y, 0xFF0000FF);
    }

    win->draw_text("TOP", SCORE_X, SCORE_Y - 120, 0xFFFFFFFF);
    std::string top_str = std::to_string(top);
    add_leading_zeros(6, top_str);
    win->draw_text(top_str, SCORE_X, SCORE_Y - 120 + 40, 0xFFFFFFFF);

    win->draw_text("SCORE", SCORE_X, SCORE_Y, 0xFFFFFFFF);
    std::string score_str = std::to_string(score);
    add_leading_zeros(6, score_str);
    win->draw_text(score_str, SCORE_X, SCORE_Y + 40, 0xFFFFFFFF);

    win->draw_text("NEXT", NEXT_FIGURE_X, NEXT_FIGURE_Y, 0xFFFFFFFF);
    draw_figure(next_figure, NEXT_FIGURE_X, NEXT_FIGURE_Y + 60);

    win->draw_text("LEVEL", LEVEL_X, LEVEL_Y, 0xFFFFFFFF);
    std::string level_str = std::to_string(level);
    add_leading_zeros(2, level_str);
    win->draw_text(level_str, LEVEL_X + 40, LEVEL_Y + 40, 0xFFFFFFFF);

    std::string lines_str = std::to_string(rows_cleared);
    add_leading_zeros(3, lines_str);
    win->draw_text("LINES-" + lines_str, LINES_X, LINES_Y, 0xFFFFFFFF);
}

void Game::draw_gameover_stripes(int count) const
{
    int heights[] = {16, 16, 8, 8};

    Uint32 colors[] = {level_colors[level % LEVEL_COLOR_COUNT][1], WHITE, level_colors[level % LEVEL_COLOR_COUNT][0], BLACK};
    int y = 0;
    for (int i = 0; i < count; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            win->set_draw_color(colors[j]);
            win->draw_rect(GRID_X, GRID_Y + y, 10 * TILE_SIZE, heights[j]);
            y += heights[j];
        }
    }
}

void Game::init_next_figure()
{
    cur_figure = next_figure;
    next_figure = next_figure_cb();
    ++statistics[cur_figure];
    figure.init(cur_figure);
    if (down_pressed)
    {
        can_move_down = false;
    }
}

void Game::clear_rows()
{
    int cleared_count = 0;
    int k = ROWS - 1;
    for (int i = ROWS - 1; i >= 0; --i)
    {
        int tile_count = 0;
        for (int j = 0; j < COLS; ++j)
        {
            if (grid[i][j])
            {
                ++tile_count;
            }
            grid[k][j] = grid[i][j];
        }
        if (tile_count < COLS)
        {
            --k;
        }
        else if (tile_count == 0)
        {
            break;
        }
        else
        {
            ++cleared_count;
        }
    }
    rows_cleared += cleared_count;
    level = rows_cleared / 10 + config.level;
    switch (cleared_count)
    {
    case 4:
        score += 1200 * (level + 1);
        break;
    case 3:
        score += 300 * (level + 1);
        break;
    case 2:
        score += 100 * (level + 1);
        break;
    case 1:
        score += 40 * (level + 1);
        break;
    }
}

void Game::set_next_figure_cb(const std::function<int()> &cb)
{
    next_figure_cb = cb;
}

void Game::start_game()
{
    score = 0;
    level = config.level;
    rows_cleared = 0;
    memset(statistics, 0, sizeof statistics);
    memset(grid, 0, sizeof grid);
    row_clear_anim->reset();
    game_over_anim->reset();
    gameover = false;
    before_start_countdown = 1000;

    cur_figure = next_figure_cb();
    next_figure = next_figure_cb();
    figure.init(cur_figure);
    ++statistics[cur_figure];
}

int Game::get_level_tex_offset() const
{
    return level % LEVEL_COLOR_COUNT * 8;
}

void Game::print_grid() const
{
    for (int i = 0; i < ROWS; ++i)
    {
        for (int j = 0; j < COLS; ++j)
        {
            std::cout << grid[i][j] ? 1 : 0;
        }
        std::cout << "\n";
    }
}

Game::Game(const GameConfig &config)
    : config(config)
{
    int w = GAME_WIDTH;
    int h = GAME_HEIGHT;
    width = w;
    height = h;
    win = std::make_unique<Window>(std::cout, w, h);
    game_over_anim = std::make_unique<GameOverAnimation>(*this);
    row_clear_anim = std::make_unique<RowClearAnimation>(*this);
}