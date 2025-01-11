#ifndef WINDOW_H
#define WINDOW_H

#include <sstream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#define TILE_SIZE 38
#define ANSI_CLEAR "\x1B[2J\x1B[H"
#define ANSI_COLOR_RESET "\x1B[m"
#define COLOR_DEFAULT "\033[0m"
#define WHITE 0xffffffff
#define BLACK 0x000000ff


struct Texture
{
    int width;
    int height;
    SDL_Texture *handle = nullptr;
    void destroy() { SDL_DestroyTexture(handle); }
};

class Window
{
public:
    Window(std::ostream &output_stream, int width, int height);
    ~Window();
    void begin_frame();
    void draw_image(const Texture &texture, int x, int y, int w, int h);
    void draw_image(const Texture &texture, int x, int y);
    void draw_rect(int x, int y, int w, int h);
    void draw_text(const std::string &text, int x, int y, Uint32 color);
    void set_draw_color(Uint32 color);
    void draw_tile(int x, int y, const Texture &texture /* char ch, const std::string &color, int repeat = 1*/);
    void draw_tiles(int x, int y, const std::string &s, const std::string &color);
    void end_frame();
    void init();
    void destroy();
    Texture generate_texture(Uint32 color, int style);
    Texture generate_text_texture(const char *text);
    Texture load_image_texture(const char *path);

private:
    Uint32 draw_color = 0xffffffff;
    SDL_Window *handle;
    SDL_Renderer *renderer;
    int width, height;
    std::pair<char, std::string> *data;
    std::ostream &output_stream;
    TTF_Font *font;
    Texture char_textures[256];
};

#endif