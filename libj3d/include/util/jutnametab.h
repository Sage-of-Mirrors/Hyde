#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <bstream.h>

namespace j3d
{
	class JUTNameTab {
		std::vector<std::string> mNames;

		uint16_t HashName(std::string name);

	public:
		JUTNameTab() {}
		~JUTNameTab() {}

		void Serialize(bStream::CStream* stream);
		void Deserialize(bStream::CStream* stream);

		std::string GetName(uint16_t index) const;
		void AddName(std::string name);
	};
} // namespace j3d
