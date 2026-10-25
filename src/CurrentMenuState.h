#pragma once
#include "Json.h"
#include <string>
namespace dvb {
template<class Menu> inline json CurrentMenuState(const std::string& name, Menu* menu)
{
    json state{{"name", name}, {"available", menu != nullptr}};
    if (menu) state.update(json{
        {"alwaysOpen", menu->AlwaysOpen()}, {"pausesGame", menu->PausesGame()},
        {"modal", menu->Modal()}, {"usesCursor", menu->UsesCursor()},
        {"usesMenuContext", menu->UsesMenuContext()}, {"freezeFramePause", menu->FreezeFramePause()}});
    return state;
}
}
