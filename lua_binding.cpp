#include "lua_binding.h"

MakeShape::MakeShape(const std::string& svg) {
    Shape s = parseSVG(svg);
    shapes.push_back(s);
    id = shapes.size()-1;
}

void MakeShape::show() { shapes[id].visible = true; }
void MakeShape::hide() { shapes[id].visible = false; }

void registerLuaBindings(sol::state& lua) {
    lua.new_usertype<MakeShape>("MakeShape",
        sol::call_constructor, sol::constructors<MakeShape(const std::string&)>(),
        "show", &MakeShape::show,
        "hide", &MakeShape::hide
    );
}
