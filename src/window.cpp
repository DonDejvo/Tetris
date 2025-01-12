#include "window.hpp"
#include <cstring>
#include <iostream>
#include "utils.hpp"

Window::Window(std::ostream &output_stream, int width, int height)
    : width(width), height(height)
{
}

void Window::begin_frame()
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}

void Window::draw_image(const Texture &texture, int x, int y, int w, int h)
{
  SDL_Rect src_rect{0, 0, texture.width, texture.height};
  SDL_Rect dst_rect{x, y, w, h};
  SDL_RenderCopy(renderer, texture.handle, &src_rect, &dst_rect);
}

void Window::draw_image(const Texture &texture, int x, int y)
{
  draw_image(texture, x, y, texture.width, texture.height);
}

void Window::draw_rect(int x, int y, int w, int h)
{
  SDL_Rect rect = {x, y, w, h};
  SDL_RenderFillRect(renderer, &rect);
}

void Window::draw_text(const std::string& text, int x, int y, Uint32 color)
{
  int offset_x = 0;
  for(int i = 0; i < text.length(); ++i) {
    Texture tex = char_textures[text[i]];
    Uint8 r, g, b, a;
    color_get_rgba(color, r, g, b, a);
    SDL_SetTextureColorMod(tex.handle, r, g, b);
    draw_image(tex, x + offset_x, y);
    offset_x += tex.width;
  }
}

void Window::set_draw_color(Uint32 color)
{
  draw_color = color;
  Uint8 r, g, b, a;
  color_get_rgba(color, r, g, b, a);
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Window::draw_tile(int x, int y, const Texture &texture /*char ch, const std::string &color, int repeat */)
{
  SDL_Rect src_rect{0, 0, texture.width, texture.height};
  SDL_Rect dst_rect{x, y, TILE_SIZE, TILE_SIZE};
  SDL_RenderCopy(renderer, texture.handle, &src_rect, &dst_rect);
}

void Window::end_frame()
{
  SDL_RenderPresent(renderer);
}

void Window::init()
{
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  handle = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(handle, -1, SDL_RENDERER_ACCELERATED);
  font = TTF_OpenFont("assets/tetris-the-absolute-the-grand-master-2.ttf", 32);
  if (!font)
  {
    std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
  }

  char chars[] = "-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  for(int i = 0; i < strlen(chars); ++i) {
    char s[2];
    s[0] = chars[i];
    s[1] = '\0';
    char_textures[chars[i]] = generate_text_texture(s);
  }
}

void Window::destroy()
{
  for(int i = 0; i < 256; ++i) {
    char_textures[i].destroy();
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(handle);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
}

Texture Window::generate_texture(Uint32 color, int style)
{
  const int tex_width = 8, tex_height = 8;
  SDL_Texture *texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_STREAMING, tex_width, tex_height);
  void *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, &pixels, &pitch);
  Uint32 *pixel_data = (Uint32 *)pixels;
  int border = style == 4 ? 0 : 1;
  for (int i = border; i < tex_height - border; ++i)
  {
    for (int j = border; j < tex_width - border; ++j)
    {
      pixel_data[i * tex_width + j] = color;
    }
  }
  switch (style)
  {
  case 1:
    pixel_data[9] = WHITE;
    pixel_data[18] = WHITE;
    pixel_data[19] = WHITE;
    pixel_data[26] = WHITE;
    break;
  case 2:
    for (int i = 2; i < tex_height - 2; ++i)
    {
      for (int j = 2; j < tex_width - 2; ++j)
      {
        pixel_data[i * tex_width + j] = WHITE;
      }
    }
    break;
  case 3:
    pixel_data[9] = WHITE;
    for (int i = 2; i < tex_height - 2; ++i)
    {
      for (int j = 2; j < tex_width - 2; ++j)
      {
        pixel_data[i * tex_width + j] = WHITE;
      }
    }
    break;
  }
  SDL_UnlockTexture(texture);
  return {tex_width, tex_height, texture};
}

Texture Window::generate_text_texture(const char *text)
{
  SDL_Surface *surface;
  SDL_Color color = {255, 255, 255};

  surface = TTF_RenderText_Solid(font, text, color);
  if (!surface)
  {
    std::cout << "Failed to render text: " << TTF_GetError() << std::endl;
  }

  int w, h;
  TTF_SizeText(font, text, &w, &h);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  return {w, h, texture};
}

Texture Window::load_image_texture(const char *path)
{
  SDL_Surface *surface = IMG_Load(path);

  int w = surface->w;
  int h = surface->h;
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_FreeSurface(surface);

  return {w, h, texture};
}
