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
bool QuitRefused(dvb::json value, const std::string& state="Main", bool selected=false)
{
    try { dvb::RequireQuitEntry(value,state,selected); return false; }
    catch (const dvb::ToolError&) { return true; }
}
dvb::json QuitReady()
{
    auto value=Ready();
    value["entries"].push_back({{"text","$QUIT"},{"index",9},{"disabled",false},{"position",4}});
    value["selected"]={{"text","$QUIT"},{"index",9},{"disabled",false}};
    return value;
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
TEST_CASE("main menu QUIT admission requires unique enabled exact native identity")
{
    auto data=QuitReady();CHECK(dvb::RequireQuitEntry(data,"Main",false)==4);
    data["state"]="MainConfirm";CHECK(QuitRefused(data));
    data=Ready();CHECK(QuitRefused(data));
    data=QuitReady();data["entries"][1]["disabled"]=true;CHECK(QuitRefused(data));
    data=QuitReady();data["entries"][1]["index"]=1;CHECK(QuitRefused(data));
    data=QuitReady();data["entries"][1]["text"]="$NEW";CHECK(QuitRefused(data));
    data=QuitReady();data["entries"].push_back(data["entries"][1]);CHECK(QuitRefused(data));
    CHECK(dvb::RequireNewEntry(QuitReady(),"Main",false)==0);
}
TEST_CASE("QUIT confirmation refuses NEW and every foreign disabled or missing selection")
{
    auto data=QuitReady();data["state"]="MainConfirm";
    CHECK(dvb::RequireQuitEntry(data,"MainConfirm",true)==4);
    CHECK(Refused(data,"MainConfirm",true));
    data["selected"]={{"text","$NEW"},{"index",1},{"disabled",false}};
    CHECK(QuitRefused(data,"MainConfirm",true));
    data=QuitReady();data["state"]="MainConfirm";data["selected"]["index"]=1;
    CHECK(QuitRefused(data,"MainConfirm",true));
    data=QuitReady();data["state"]="MainConfirm";data["selected"]["disabled"]=true;
    CHECK(QuitRefused(data,"MainConfirm",true));
    data["selected"]=nullptr;CHECK(QuitRefused(data,"MainConfirm",true));
    data=QuitReady();CHECK(QuitRefused(data,"MainConfirm",true));
}
