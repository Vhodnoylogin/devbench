#include "test_framework.h"
#include "TextEncoding.h"
#include "ToolRegistry.h"
#include "McpContent.h"

using dvb::json;

TEST_CASE("invalid engine name cannot prevent scene identity serialization")
{
	json value{ { "playerLoaded", true }, { "cell", { { "editorId", "QASmoke" },
		{ "formId", "0x00032AE7" }, { "name", std::string(1, char(0xC7)) } } },
		{ "position", { 1.25, -2.0, 3.0 } } };
	CHECK_THROWS(value.dump());  // reproduces the actual incomplete UTF-8 failure
	auto safe = dvb::NormalizeToolText(value);
	CHECK(safe["cell"]["name"].is_null());
	CHECK(safe["cell"]["editorId"] == "QASmoke");
	CHECK(safe["position"] == value["position"]);
	CHECK(safe["playerLoaded"] == true);
	CHECK(safe["textEncodingDiagnostics"]["records"][0]["path"] == "/cell/name");
	CHECK(safe["textEncodingDiagnostics"]["records"][0]["rawHex"] == "c7");
	CHECK(json::parse(safe.dump()) == safe);
}

TEST_CASE("valid Russian UTF-8 and null numeric values remain byte identical")
{
	json value{ { "name", "Зелье лечения" }, { "x", 1.5 }, { "missing", nullptr }, { "nul", std::string("a\0b", 3) } };
	CHECK(dvb::NormalizeToolText(value) == value);
	CHECK(dvb::NormalizeToolText(json::array({ "Зелье", 2 })) == json::array({ "Зелье", 2 }));
}

TEST_CASE("diagnostics use escaped JSON pointers and cover nested arrays")
{
	json value{ { "a/b~c", json::array({ { { "name", std::string("\xff", 1) } } }) } };
	auto safe = dvb::NormalizeToolText(value);
	CHECK(safe["textEncodingDiagnostics"]["records"][0]["path"] == "/a~1b~0c/0/name");
	CHECK(safe["a/b~c"][0]["name"].is_null());
}

TEST_CASE("large raw names and many invalid strings have explicit bounded diagnostics")
{
	json value{ { "names", json::array() } };
	for (int i = 0; i < 35; ++i)
		value["names"].push_back(std::string(256, char(0xFF)));
	auto safe = dvb::NormalizeToolText(value);
	auto d = safe["textEncodingDiagnostics"];
	CHECK(d["invalidStringCount"] == 35);
	CHECK(d["records"].size() == 32);
	CHECK(d["recordsTruncated"] == true);
	CHECK(d["records"][0]["rawHex"].get<std::string>().size() == 256);
	CHECK(d["records"][0]["byteLength"] == 256);
	CHECK(d["records"][0]["rawTruncated"] == true);
	CHECK(safe["names"][34].is_null());
}

TEST_CASE("invalid object keys scalar roots and reserved diagnostics are never silently rewritten")
{
	CHECK_THROWS(dvb::NormalizeToolText(json{ { std::string("\xff", 1), 2 } }));
	CHECK_THROWS(dvb::NormalizeToolText(json(std::string("\xc7", 1))));
	CHECK_THROWS(dvb::NormalizeToolText(json{ { "bad", std::string("\xc7", 1) }, { "textEncodingDiagnostics", 7 } }));
}

TEST_CASE("overlong surrogate and out-of-range UTF-8 are unavailable")
{
	for (const auto& raw : { std::string("\xc0\xaf", 2), std::string("\xed\xa0\x80", 3), std::string("\xf4\x90\x80\x80", 4) }) {
		auto safe = dvb::NormalizeToolText(json{ { "name", raw } });
		CHECK(safe["name"].is_null());
		CHECK(safe["textEncodingDiagnostics"]["invalidStringCount"] == 1);
	}
}

TEST_CASE("tool result normalization never repeats mutation and both adapters receive safe payload")
{
	dvb::ToolRegistry reg;
	dvb::ToolDescriptor desc;
	desc.name = "inspect";
	int calls = 0;
	reg.Register(desc, [&](const json&, const dvb::ToolContext&) {
		++calls;
		return json{ { "name", std::string("\xc7", 1) }, { "formId", "0x14" } };
	});
	auto result = reg.Invoke("inspect", json::object(), {});
	CHECK(calls == 1);
	CHECK(result.ok);
	CHECK(result.value["name"].is_null());
	CHECK(result.value["formId"] == "0x14");
	CHECK(json::parse(result.value.dump()) == result.value);
	auto content = dvb::ToContentBlocks(result.value);
	CHECK(json::parse(content[0]["text"].get<std::string>()) == result.value);
}
