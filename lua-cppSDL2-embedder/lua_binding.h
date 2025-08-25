#pragma once
#include <sol/sol.hpp>
#include "shape.h"

class MakeShape {
public:
    int id;
    MakeShape(const std::string& svg);
    void show();
    void hide();
};

void registerLuaBindings(sol::state& lua);
