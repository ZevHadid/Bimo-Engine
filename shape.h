#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>

struct Shape {
    enum Type { RECT, CIRCLE, LINE, POLYGON } type;
    std::vector<SDL_Point> points;
    int x=0, y=0, w=0, h=0;
    int cx=0, cy=0, r=0;
    Uint8 r_col=255, g_col=255, b_col=255;
    bool visible = false;
};

extern std::vector<Shape> shapes;

Shape parseSVG(const std::string &svg);
