#include "test_framework.h"
#include "StartupMenuLogic.h"
namespace {
dvb::json Ready()
{
    return {{"state", "Main"}, {"entries", {{{"text", "$NEW"}, {"index", 1}, {"disabled", false}, {"position", 0}}}},
        {"selected", {{"text", "$NEW"}, {"index", 1}, {"disabled", false}}}};
}
bool Refused(dvb::json value, const std::string& state="Main", bool selected=false)
{
    try { dvb::RequireNewEntry(value,state,selected); return false; }
    catch (const dvb::ToolError&) { return true; }
}
}
TEST_CASE("startup NEW admission requires actual state and unique enabled entry")
{
    auto data=Ready(); CHECK(dvb::RequireNewEntry(data,"Main",false)==0);
    data["state"]="MainConfirm"; CHECK(Refused(data));
    data=Ready();data["entries"]=dvb::json::array();CHECK(Refused(data));
    data=Ready();data["entries"][0]["disabled"]=true;CHECK(Refused(data));
    data=Ready();data["entries"][0]["text"]="$CONTINUE";CHECK(Refused(data));
    data=Ready();data["entries"].push_back(data["entries"][0]);CHECK(Refused(data));
}
TEST_CASE("startup confirmation refuses wrong disabled or missing selected item")
{
    auto data=Ready();data["state"]="MainConfirm";CHECK(dvb::RequireNewEntry(data,"MainConfirm",true)==0);
    data["selected"]["index"]=0;CHECK(Refused(data,"MainConfirm",true));
    data=Ready();data["state"]="MainConfirm";data["selected"]["disabled"]=true;CHECK(Refused(data,"MainConfirm",true));
    data["selected"]=nullptr;CHECK(Refused(data,"MainConfirm",true));
}
