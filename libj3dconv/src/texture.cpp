#include "texture.hpp"
#include "jutnametab.hpp"
#include "util.hpp"

#include "bstream.h"

#include <tiny_gltf.h>

/* CTextureData */

CTextureData::CTextureData() {

}

CTextureData::~CTextureData() {
    mTextures.clear();
}

EWrapMode CTextureData::ConvertWrapMode(int mode) {
    switch (mode) {
        case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
            return EWrapMode::Clamp;
        case TINYGLTF_TEXTURE_WRAP_REPEAT:
            return EWrapMode::Repeat;
        case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
            return EWrapMode::Mirror;
        default:
            return EWrapMode::Clamp;
    }
}

EFilterMode CTextureData::ConvertFilterMode(int mode) {
    switch (mode) {
        case TINYGLTF_TEXTURE_FILTER_NEAREST:
            return EFilterMode::Nearest;
        case TINYGLTF_TEXTURE_FILTER_LINEAR:
            return EFilterMode::Linear;
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
            return EFilterMode::NearestMipmapNearest;
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
            return EFilterMode::NearestMipmapLinear;
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
            return EFilterMode::LinearMipmapNearest;
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
            return EFilterMode::LinearMipmapLinear;
    }
}

void CTextureData::ProcessTextureData(const tinygltf::Model* model, std::vector<bStream::CMemoryStream>& buffers) {
    for (const tinygltf::Texture& tex : model->textures) {
        const tinygltf::Image& img = model->images[tex.source];
        const tinygltf::Sampler& smp = model->samplers[tex.sampler];

        std::shared_ptr<STexture> newTexture = std::make_shared<STexture>();
        newTexture->mName = img.name;
        newTexture->mData = img.image;
        newTexture->mWidth = img.width;
        newTexture->mHeight = img.height;

        newTexture->mWrapS = ConvertWrapMode(smp.wrapS);
        newTexture->mWrapT = ConvertWrapMode(smp.wrapT);
        newTexture->mFilterMin = ConvertFilterMode(smp.minFilter);
        newTexture->mFilterMag = ConvertFilterMode(smp.magFilter);

        // Disable alpha by default
        newTexture->mPaletteFormat = EPaletteFormat::RGB565;

        if (img.component == 4) {
            // Check the alpha component of each pixel to see if it's less than 0xFF
            for (uint32_t i = 3; i < img.image.size(); i += 4) {
                if (img.image[i] < 0xFF) {
                    // If the alpha is less than 0xFF, enable alpha and leave the loop
                    newTexture->mPaletteFormat = EPaletteFormat::RGB5A3;
                    break;
                }
            }
        }

        mTextures.push_back(newTexture);
    }
}

void CTextureData::WriteTEX1(bStream::CStream& stream) {
    JUTNameTab textureNameTable;
    size_t streamStartPos = stream.tell();

    // Header
    stream.writeUInt32(0x54455831);       // FourCC ('TEX1')
    stream.writeUInt32(0);                // Placeholder for section size
    stream.writeUInt16(mTextures.size()); // Number of textures
    stream.writeUInt16(UINT16_MAX);       // Padding

    stream.writeUInt32(0x20); // Offset to image data, always 0x20
    stream.writeUInt32(0);    // Placeholder for offset to name table

    Util::PadStreamWithString(&stream, 32);

    for (auto& tex : mTextures) {
        textureNameTable.AddName(tex->mName);
    }

    // Write name table offset
    Util::WriteOffset(&stream, streamStartPos, 0x10);
    // Write name table
    textureNameTable.Serialize(&stream);

    Util::PadStreamWithString(&stream, 32);

    // Write section size
    Util::WriteOffset(&stream, streamStartPos, 0x04);
}
