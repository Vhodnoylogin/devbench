#pragma once

#include "Json.h"

#include <algorithm>
#include <stdexcept>
#include <string>

namespace dvb
{
	// Engine strings may be legacy encoded. Their encoding cannot be inferred
	// from one bad byte or the Windows locale. Preserve identities/numeric data,
	// explicitly mark unreadable text null, and retain bounded original bytes.
	// This is a tool-result boundary shared by REST, MCP and internal scenarios;
	// it never re-dispatches the handler or alters supplied arguments.
	inline bool IsUtf8(const std::string& a_text)
	{
		try {
			(void)json(a_text).dump();
			return true;
		} catch (const json::type_error& e) {
			if (e.id != 316)
				throw;
			return false;
		}
	}

	inline std::string JsonPointerSegment(const std::string& a_key)
	{
		std::string out;
		for (const char c : a_key)
			out += c == '~' ? "~0" : c == '/' ? "~1" : std::string(1, c);
		return out;
	}

	inline json NormalizeToolText(json a_result)
	{
		// Fast path also validates object keys. Ordinary UTF-8 results keep their
		// exact shape/content and have no added fields.
		try {
			(void)a_result.dump();
			return a_result;
		} catch (const json::type_error& e) {
			if (e.id != 316)
				throw;
		}
		if (!a_result.is_object() || a_result.contains("textEncodingDiagnostics"))
			throw std::runtime_error("invalid UTF-8 in tool output: cannot attach textEncodingDiagnostics without changing result shape");

		constexpr std::size_t kMaxRecords = 32;
		constexpr std::size_t kRawPrefixBytes = 128;
		json records = json::array();
		std::size_t invalidCount = 0;
		auto visit = [&](auto&& self, json& value, const std::string& path) -> void {
			if (value.is_string()) {
				const auto& raw = value.get_ref<const std::string&>();
				if (IsUtf8(raw))
					return;
				++invalidCount;
				if (records.size() < kMaxRecords) {
					constexpr char hex[] = "0123456789abcdef";
					std::string bytes;
					for (std::size_t i = 0; i < std::min(raw.size(), kRawPrefixBytes); ++i) {
						const auto c = static_cast<unsigned char>(raw[i]);
						bytes += hex[c >> 4];
						bytes += hex[c & 15];
					}
					records.push_back(json{ { "path", path }, { "rawHex", bytes },
						{ "byteLength", raw.size() }, { "rawTruncated", raw.size() > kRawPrefixBytes } });
				}
				value = nullptr;  // unavailable text, never a guessed/lossy decoded name
			} else if (value.is_object()) {
				for (auto& [key, child] : value.items()) {
					if (!IsUtf8(key))
						throw std::runtime_error("invalid UTF-8 in tool output object key: result refused rather than renaming keys");
					self(self, child, path + '/' + JsonPointerSegment(key));
				}
			} else if (value.is_array()) {
				for (std::size_t i = 0; i < value.size(); ++i)
					self(self, value[i], path + '/' + std::to_string(i));
			}
		};
		visit(visit, a_result, "");
		a_result["textEncodingDiagnostics"] = json{
			{ "policy", "invalid-utf8-text-unavailable" }, { "sourceEncoding", "unknown" },
			{ "invalidStringCount", invalidCount }, { "records", std::move(records) },
			{ "recordsTruncated", invalidCount > kMaxRecords }
		};
		return a_result;
	}
}
