#include "StartupMenu.h"
#include "StartupMenuLogic.h"
#include "MainThread.h"
#include <cmath>
namespace dvb {
namespace {
constexpr auto root = "_root.MenuHolder.Menu_mc";
json Read(RE::GFxMovieView* movie, const std::string& path)
{
    RE::GFxValue v;
    if (!movie->GetVariable(&v, path.c_str()) || v.IsUndefined() || v.IsNull()) return nullptr;
    if (v.IsBool()) return v.GetBool();
    if (v.IsString()) return std::string(v.GetString());
    if (v.IsNumber() && std::isfinite(v.GetNumber()) && std::floor(v.GetNumber()) == v.GetNumber() &&
        std::abs(v.GetNumber()) < 10000) return static_cast<int>(v.GetNumber());
    throw ToolError(503, "Unexpected startup movie value type");
}
json Snapshot(RE::GFxMovieView* movie)
{
    const std::string base(root);
    json result{{"contract", "devbench-startup-ui/1"}, {"menu", "Main Menu"},
        {"state", Read(movie, base + ".strCurrentState")}, {"entries", json::array()}, {"selected", nullptr}};
    if (!result["state"].is_string()) throw ToolError(503, "Identified startup movie state unavailable");
    if (result["state"] != "Main" && result["state"] != "MainConfirm") return result;
    auto count = Read(movie, base + ".MainList.entryList.length");
    if (!count.is_number_integer() || count.get<int>() < 1 || count.get<int>() > 12)
        throw ToolError(503, "Startup list metadata unavailable");
    for (int i = 0; i < count.get<int>(); ++i) {
        auto path = base + ".MainList.entryList." + std::to_string(i);
        auto entry = json{{"position", i}, {"text", Read(movie, path + ".text")},
            {"index", Read(movie, path + ".index")}, {"disabled", Read(movie, path + ".disabled")}};
        if (!entry["text"].is_string() || !entry["index"].is_number_integer() || !entry["disabled"].is_boolean())
            throw ToolError(503, "Startup entry metadata unavailable");
        result["entries"].push_back(std::move(entry));
    }
    auto path = base + ".MainList.selectedEntry";
    auto index = Read(movie, path + ".index");
    if (!index.is_null()) {
        result["selected"] = json{{"index", index}, {"text", Read(movie, path + ".text")},
            {"disabled", Read(movie, path + ".disabled")}};
        if (!index.is_number_integer() || !result["selected"]["text"].is_string() ||
            !result["selected"]["disabled"].is_boolean()) throw ToolError(503, "Startup selection metadata unavailable");
    }
    return result;
}
}
json StartupMenu(const json& args)
{
    if (args.size() != 2 || !args.contains("action") || !args.contains("startupOperation") || !args["startupOperation"].is_string())
        throw ToolError(400, "startup requires only action and startupOperation");
    const auto operation = args["startupOperation"].get<std::string>();
    if (operation != "snapshot" && operation != "select-new" && operation != "confirm-new")
        throw ToolError(400, "startup operation must be snapshot, select-new or confirm-new");
    return MainThread::RunAndWait([operation]() -> json {
        auto* ui = RE::UI::GetSingleton();
        const auto menu = ui ? ui->GetMenu("Main Menu") : nullptr;
        if (!menu || !menu->uiMovie || !ui->IsMenuOpen("Main Menu"))
            throw ToolError(409, "Owned startup menu is not open");
        auto* movie = menu->uiMovie.get();
        auto before = Snapshot(movie);
        if (operation == "snapshot") return before;
        const auto position = RequireNewEntry(before, operation == "select-new" ? "Main" : "MainConfirm", operation == "confirm-new");
        RE::GFxValue result;
        if (operation == "select-new") {
            if (!movie->SetVariable((std::string(root) + ".MainList.selectedIndex").c_str(), RE::GFxValue(position)))
                throw ToolError(503, "Startup selection write unavailable");
            RequireNewEntry(Snapshot(movie), "Main", true);
            RE::GFxValue key(0);
            if (!movie->Invoke((std::string(root) + ".MainList.onItemPress").c_str(), &result, &key, 1))
                throw ToolError(503, "Startup NEW invocation unavailable; do not replay");
        } else if (!movie->Invoke((std::string(root) + ".onAcceptPress").c_str(), &result, nullptr, 0)) {
            throw ToolError(503, "Startup confirmation invocation unavailable; do not replay");
        }
        return json{{"contract", "devbench-startup-ui/1"}, {"operation", operation}, {"completed", true},
            {"before", before}, {"basis", "identified native movie invocation completed; not a loaded world or SKSE lifecycle proof"}};
    });
}
}
