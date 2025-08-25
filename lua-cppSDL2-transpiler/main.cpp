#include <SDL2/SDL.h>
#include <sol/sol.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>

// ---------------------- Shape Struct ----------------------
struct Shape {
    enum Type { RECT, CIRCLE, LINE, POLYGON } type;
    std::vector<SDL_Point> points;
    int x=0, y=0, w=0, h=0;
    int cx=0, cy=0, r=0;
    Uint8 r_col=255, g_col=255, b_col=255;
    bool visible = false;
};

// Global shapes vector
std::vector<Shape> shapes;

// ---------------------- Helper: parse simple SVG ----------------------
Shape parseSVG(const std::string &svg) {
    Shape s;

    if (svg.find("<rect") != std::string::npos) {
        s.type = Shape::RECT;
        size_t x_pos = svg.find("x=\"");
        size_t y_pos = svg.find("y=\"");
        size_t w_pos = svg.find("width=\"");
        size_t h_pos = svg.find("height=\"");

        s.x = std::stoi(svg.substr(x_pos+3, svg.find("\"", x_pos+3)-x_pos-3));
        s.y = std::stoi(svg.substr(y_pos+3, svg.find("\"", y_pos+3)-y_pos-3));
        s.w = std::stoi(svg.substr(w_pos+7, svg.find("\"", w_pos+7)-w_pos-7));
        s.h = std::stoi(svg.substr(h_pos+8, svg.find("\"", h_pos+8)-h_pos-8));

        size_t fill_pos = svg.find("fill=\"");
        if(fill_pos != std::string::npos){
            std::string color = svg.substr(fill_pos+6, svg.find("\"", fill_pos+6)-fill_pos-6);
            if(color == "blue") s.r_col=0, s.g_col=0, s.b_col=255;
            else if(color == "red") s.r_col=255, s.g_col=0, s.b_col=0;
            else if(color == "green") s.r_col=0, s.g_col=255, s.b_col=0;
            else if(color == "gold") s.r_col=255, s.g_col=215, s.b_col=0;
        }
    }
    else if(svg.find("<circle") != std::string::npos){
        s.type = Shape::CIRCLE;
        size_t cx_pos = svg.find("cx=\"");
        size_t cy_pos = svg.find("cy=\"");
        size_t r_pos  = svg.find("r=\"");

        s.cx = std::stoi(svg.substr(cx_pos+4, svg.find("\"", cx_pos+4)-cx_pos-4));
        s.cy = std::stoi(svg.substr(cy_pos+4, svg.find("\"", cy_pos+4)-cy_pos-4));
        s.r  = std::stoi(svg.substr(r_pos+3, svg.find("\"", r_pos+3)-r_pos-3));

        size_t fill_pos = svg.find("fill=\"");
        if(fill_pos != std::string::npos){
            std::string color = svg.substr(fill_pos+6, svg.find("\"", fill_pos+6)-fill_pos-6);
            if(color == "blue") s.r_col=0, s.g_col=0, s.b_col=255;
            else if(color == "red") s.r_col=255, s.g_col=0, s.b_col=0;
            else if(color == "green") s.r_col=0, s.g_col=255, s.b_col=0;
            else if(color == "gold") s.r_col=255, s.g_col=215, s.b_col=0;
        }
    }
    else if(svg.find("<line") != std::string::npos){
        s.type = Shape::LINE;
        size_t x1_pos = svg.find("x1=\"");
        size_t y1_pos = svg.find("y1=\"");
        size_t x2_pos = svg.find("x2=\"");
        size_t y2_pos = svg.find("y2=\"");

        SDL_Point p1, p2;
        p1.x = std::stoi(svg.substr(x1_pos+4, svg.find("\"", x1_pos+4)-x1_pos-4));
        p1.y = std::stoi(svg.substr(y1_pos+4, svg.find("\"", y1_pos+4)-y1_pos-4));
        p2.x = std::stoi(svg.substr(x2_pos+4, svg.find("\"", x2_pos+4)-x2_pos-4));
        p2.y = std::stoi(svg.substr(y2_pos+4, svg.find("\"", y2_pos+4)-y2_pos-4));
        s.points.push_back(p1);
        s.points.push_back(p2);

        size_t stroke_pos = svg.find("stroke=\"");
        if(stroke_pos != std::string::npos){
            std::string color = svg.substr(stroke_pos+8, svg.find("\"", stroke_pos+8)-stroke_pos-8);
            if(color == "blue") s.r_col=0, s.g_col=0, s.b_col=255;
            else if(color == "red") s.r_col=255, s.g_col=0, s.b_col=0;
            else if(color == "green") s.r_col=0, s.g_col=255, s.b_col=0;
        }
    }
    else if(svg.find("<polygon") != std::string::npos){
        s.type = Shape::POLYGON;
        size_t points_pos = svg.find("points=\"");
        std::string pts_str = svg.substr(points_pos+8, svg.find("\"", points_pos+8)-points_pos-8);
        std::stringstream ss(pts_str);
        std::string pair;
        while(std::getline(ss, pair, ' ')){
            size_t comma = pair.find(",");
            int x = std::stoi(pair.substr(0, comma));
            int y = std::stoi(pair.substr(comma+1));
            s.points.push_back({x,y});
        }
        size_t fill_pos = svg.find("fill=\"");
        if(fill_pos != std::string::npos){
            std::string color = svg.substr(fill_pos+6, svg.find("\"", fill_pos+6)-fill_pos-6);
            if(color == "blue") s.r_col=0, s.g_col=0, s.b_col=255;
            else if(color == "red") s.r_col=255, s.g_col=0, s.b_col=0;
            else if(color == "green") s.r_col=0, s.g_col=255, s.b_col=0;
            else if(color == "gold") s.r_col=255, s.g_col=215, s.b_col=0;
        }
    }

    return s;
}

// ---------------------- Lua class binding ----------------------
class MakeShape {
public:
    int id;
    MakeShape(const std::string& svg) {
        Shape s = parseSVG(svg);
        shapes.push_back(s);
        id = shapes.size()-1;
    }
    void show() { shapes[id].visible = true; }
    void hide() { shapes[id].visible = false; }
};

// ---------------------- Main ----------------------
int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Lua SDL2 Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::package);

    // Register C++ class for Lua - CORRECTED WAY
    lua.new_usertype<MakeShape>("MakeShape",
        sol::call_constructor, sol::constructors<MakeShape(const std::string&)>(),
        "show", &MakeShape::show,
        "hide", &MakeShape::hide
    );

    // Run Lua script
    try {
        lua.script_file("script.lua");
    } catch (const sol::error& e) {
        std::cerr << "Lua error: " << e.what() << std::endl;
        return 1;
    }

    bool quit = false;
    SDL_Event e;
    while(!quit){
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) quit = true;
        }

        SDL_SetRenderDrawColor(ren, 0,0,0,255);
        SDL_RenderClear(ren);

        for(auto &s : shapes){
            if(!s.visible) continue;
            SDL_SetRenderDrawColor(ren, s.r_col, s.g_col, s.b_col, 255);
            switch(s.type){
                case Shape::RECT: {
                    SDL_Rect r{s.x, s.y, s.w, s.h};
                    SDL_RenderFillRect(ren, &r);
                    break;
                }
                case Shape::CIRCLE: {
                    // More efficient circle drawing
                    for (int y = -s.r; y <= s.r; y++) {
                        for (int x = -s.r; x <= s.r; x++) {
                            if (x*x + y*y <= s.r*s.r) {
                                SDL_RenderDrawPoint(ren, s.cx + x, s.cy + y);
                            }
                        }
                    }
                    break;
                }
                case Shape::LINE:
                    SDL_RenderDrawLine(ren, s.points[0].x, s.points[0].y, s.points[1].x, s.points[1].y);
                    break;
                case Shape::POLYGON:
                    SDL_RenderDrawLines(ren, s.points.data(), s.points.size());
                    // Close the polygon by drawing a line from last to first point
                    if (s.points.size() > 1) {
                        SDL_RenderDrawLine(ren, 
                            s.points.back().x, s.points.back().y,
                            s.points.front().x, s.points.front().y);
                    }
                    break;
            }
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16); // Add a small delay to reduce CPU usage
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
