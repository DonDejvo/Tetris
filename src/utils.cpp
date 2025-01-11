#include "utils.hpp"

void add_leading_zeros(int final_length, std::string& str) {
    while(str.length() < final_length) {
        str = '0' + str;
    }
}

void color_get_rgba(Uint32 color, Uint8& r, Uint8& g, Uint8& b, Uint8& a) {
    r = (color >> 24) & 0xFF;
    g = (color >> 16) & 0xFF;
    b = (color >> 8) & 0xFF;
    a = (color) & 0xFF;
}