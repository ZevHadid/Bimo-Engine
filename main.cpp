#include <SDL2/SDL.h>
#include <sol/sol.hpp>
#include "embedded_script.h"
#include "shape.h"
#include "lua_binding.h"
#include <iostream>

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
        lua.script(EMBEDDED_LUA_SCRIPT);
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
