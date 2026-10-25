#include "test_framework.h"
#include "CurrentMenuState.h"
namespace {
struct Menu {
    bool a, p, m, c, x, f;
    bool AlwaysOpen() const { return a; }
    bool PausesGame() const { return p; }
    bool Modal() const { return m; }
    bool UsesCursor() const { return c; }
    bool UsesMenuContext() const { return x; }
    bool FreezeFramePause() const { return f; }
};
}
TEST_CASE("current menu state keeps missing objects explicitly unavailable")
{
    auto state = dvb::CurrentMenuState("Unknown", static_cast<Menu*>(nullptr));
    CHECK(state == dvb::json({{"name", "Unknown"}, {"available", false}}));
}
TEST_CASE("current menu state preserves every independent engine flag")
{
    for (unsigned int mask = 0; mask < 64; ++mask) {
        Menu menu{bool(mask&1), bool(mask&2), bool(mask&4), bool(mask&8), bool(mask&16), bool(mask&32)};
        auto state = dvb::CurrentMenuState("Overlay", &menu);
        CHECK(state["available"] == true);
        CHECK(state["alwaysOpen"] == menu.a);
        CHECK(state["pausesGame"] == menu.p);
        CHECK(state["modal"] == menu.m);
        CHECK(state["usesCursor"] == menu.c);
        CHECK(state["usesMenuContext"] == menu.x);
        CHECK(state["freezeFramePause"] == menu.f);
        CHECK(dvb::json::parse(state.dump()) == state);
    }
}
