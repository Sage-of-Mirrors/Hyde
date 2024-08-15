#include "util.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <bstream.h>

namespace Util {
	const std::string PADDING_STRING = "This is padding data to alignm";

	std::string LoadTextFile(std::filesystem::path filePath) {
		if (filePath.empty() || !std::filesystem::exists(filePath))
			return "";

		std::fstream shaderFile(filePath);
		if (!shaderFile.is_open()) {
			std::cout << "Unable to load text from " << filePath << std::endl;
			return "";
		}

		std::stringstream iss;
		iss << shaderFile.rdbuf();
		std::string shaderTxt = iss.str();

		shaderFile.close();

		return shaderTxt;
	}

	void PadStreamWithString(bStream::CStream* stream, uint32_t padValue, std::string str) {
		uint32_t nextAligned = (stream->tell() + (padValue - 1)) & ~(padValue - 1);
		uint32_t delta = nextAligned - stream->tell();

		std::string paddingString = str.empty() ? PADDING_STRING : str;

		for (int i = 0; i < delta; i++) {
			stream->writeUInt8(paddingString[i % paddingString.size()]);
		}
	}

	void WriteOffset(bStream::CStream* stream, size_t relativeTo, uint32_t location) {
		size_t currentStreamPos = stream->tell();

		stream->seek(relativeTo + location);
		stream->writeUInt32(static_cast<uint32_t>(currentStreamPos - relativeTo));
		stream->seek(currentStreamPos);
	}
}
