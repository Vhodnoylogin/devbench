#pragma once
#include "ToolRegistry.h"
namespace dvb {
inline int RequireNewEntry(const json& snapshot, const std::string& state, bool selected)
{
    if (snapshot.at("state") != state) throw ToolError(409, "Main menu state changed; no startup mutation");
    int position = -1;
    for (const auto& entry : snapshot.at("entries")) {
        if (entry.at("text") == "$NEW" && entry.at("index") == 1 && entry.at("disabled") == false) {
            if (position != -1) throw ToolError(409, "Ambiguous NEW entry; no startup mutation");
            position = entry.at("position").get<int>();
        }
    }
    if (position == -1) throw ToolError(409, "Unique enabled NEW entry unavailable");
    if (selected && (snapshot.at("selected").is_null() || snapshot.at("selected").at("text") != "$NEW" ||
        snapshot.at("selected").at("index") != 1 || snapshot.at("selected").at("disabled") != false))
        throw ToolError(409, "Selected confirmation is not NEW; no startup mutation");
    return position;
}
}
