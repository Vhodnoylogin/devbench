#pragma once
#include "ToolRegistry.h"
namespace dvb {
inline int RequireStartupEntry(const json& snapshot, const std::string& state, bool selected,
                              const char* text, int index)
{
    if (snapshot.at("state") != state) throw ToolError(409, "Main menu state changed; no startup mutation");
    int position = -1;
    for (const auto& entry : snapshot.at("entries")) {
        if (entry.at("text") == text && entry.at("index") == index && entry.at("disabled") == false) {
            if (position != -1) throw ToolError(409, "Ambiguous requested entry; no startup mutation");
            position = entry.at("position").get<int>();
        }
    }
    if (position == -1) throw ToolError(409, "Unique enabled requested entry unavailable");
    if (selected && (snapshot.at("selected").is_null() || snapshot.at("selected").at("text") != text ||
        snapshot.at("selected").at("index") != index || snapshot.at("selected").at("disabled") != false))
        throw ToolError(409, "Selected confirmation is not the requested entry; no startup mutation");
    return position;
}
inline int RequireNewEntry(const json& snapshot, const std::string& state, bool selected)
{
    return RequireStartupEntry(snapshot, state, selected, "$NEW", 1);
}
inline int RequireQuitEntry(const json& snapshot, const std::string& state, bool selected)
{
    return RequireStartupEntry(snapshot, state, selected, "$QUIT", 9);
}
}
