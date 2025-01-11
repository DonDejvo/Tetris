#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <SDL_stdinc.h>

void add_leading_zeros(int final_length, std::string& str);

void color_get_rgba(Uint32 color, Uint8& r, Uint8& g, Uint8& b, Uint8& a);

#endif
