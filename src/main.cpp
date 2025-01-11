#include "argparser.hpp"
#include "game.hpp"
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

Game g;

void loop() {
    g.loop();
}

int main(int argc, char *argv[])
{
    srand(time(0));
    ProgramArguments params;
    bool success = ArgParser::parseProgramArguments(argc, argv, params);
    if (params.help || !success)
    {
        Game::help();
    }
    else
    {
        GameConfig config;
        config.level = params.level;
        g.init();

#ifdef EMSCRIPTEN
        emscripten_set_main_loop(loop, -1, 1);
#else
        while (g.is_running())
        {
            g.loop();
        }
#endif

        g.destroy();
    }
    return 0;
}
