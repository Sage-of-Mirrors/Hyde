#include "util/jutnametab.h"
#include "util/util.h"

#include <bstream.h>

const uint16_t HEADER_SIZE = 4;
const uint16_t ENTRY_SIZE = 4;

uint16_t j3d::JUTNameTab::HashName(std::string name) {
	uint16_t hash = 0;

	for (char c : name) {
		hash = hash * 3 + c;
	}

	return hash;
}

void j3d::JUTNameTab::Serialize(bStream::CStream* stream) {
	stream->writeUInt16(static_cast<uint16_t>(mNames.size()));
	stream->writeUInt16(UINT16_MAX);

	uint16_t runningOffset = static_cast<uint16_t>(HEADER_SIZE + mNames.size() * ENTRY_SIZE);

	for (std::string name : mNames) {
		stream->writeUInt16(HashName(name));
		stream->writeUInt16(runningOffset);

		runningOffset += static_cast<uint16_t>(name.length() + 1);
	}

	for (std::string name : mNames) {
		stream->writeString(name);
		stream->writeUInt8(0);
	}

	Util::PadStreamWithString(stream, 4);
}

void j3d::JUTNameTab::Deserialize(bStream::CStream* stream) {
	uint32_t tableStartPos = static_cast<uint32_t>(stream->tell());

	uint16_t count = stream->readUInt16();
	stream->skip(2);

	for (int i = 0; i < count; i++) {
		stream->skip(2);
		uint16_t stringOffset = stream->readUInt16();

		char buffer[64];
		for (int i = 0; i < 64; i++) {
			buffer[i] = stream->peekUInt8(tableStartPos + stringOffset);
			if (buffer[i] == 0)
				break;

			stringOffset++;
		}

		mNames.push_back(std::string(buffer));
	}
}

std::string j3d::JUTNameTab::GetName(uint16_t index) const {
	if (index >= 0 && index < mNames.size())
		return mNames[index];

	return "";
}

void j3d::JUTNameTab::AddName(std::string name) {
	mNames.push_back(name);
}
