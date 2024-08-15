#pragma once

#include "types.hpp"

#include <vector>
#include <string>

enum class EWrapMode {
    Clamp,
    Repeat,
    Mirror
};

enum class EPaletteFormat {
    IA8,
    RGB565,
    RGB5A3,

    None
};

enum class EFilterMode {
    Nearest,
    Linear,

    NearestMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapNearest,
    LinearMipmapLinear
};

struct STexture {
    std::string mName;
    std::vector<uint8_t> mData;
    size_t mWidth;
    size_t mHeight;

    EWrapMode mWrapS = EWrapMode::Clamp;
    EWrapMode mWrapT = EWrapMode::Clamp;

    EFilterMode mFilterMin = EFilterMode::Linear;
    EFilterMode mFilterMag = EFilterMode::Linear;

    EPaletteFormat mPaletteFormat = EPaletteFormat::None;
};

class CTextureData {
    shared_vector<STexture> mTextures;

    EWrapMode ConvertWrapMode(int mode);
    EFilterMode ConvertFilterMode(int mode);

public:
    CTextureData();
    ~CTextureData();

    void ProcessTextureData(const tinygltf::Model* model, std::vector<bStream::CMemoryStream>& buffers);

    void WriteTEX1(bStream::CStream& stream);
};
