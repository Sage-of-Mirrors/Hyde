#include "vertex.hpp"
#include "shape.hpp"
#include "util.hpp"

#include "j3denum.hpp"

#include <bstream.h>

#include <algorithm>

const uint8_t FIXED_POINT_EXP_NORMAL = 0x0E;
const uint8_t FIXED_POINT_EXP_TEXCOORD = 0x08;

/* SVertex */

void SVertex::SetIndex(EGXAttribute attribute, uint16_t index) {
    switch (attribute) {
    case EGXAttribute::Position:
        PositionIndex = index;
        break;
    case EGXAttribute::Normal:
        NormalIndex = index;
        break;
    case EGXAttribute::Color0:
    case EGXAttribute::Color1:
    {
        uint32_t colorIdx = static_cast<uint32_t>(attribute) - static_cast<uint32_t>(EGXAttribute::Color0);
        ColorIndex[colorIdx] = index;
        break;
    }
    case EGXAttribute::TexCoord0:
    case EGXAttribute::TexCoord1:
    case EGXAttribute::TexCoord2:
    case EGXAttribute::TexCoord3:
    case EGXAttribute::TexCoord4:
    case EGXAttribute::TexCoord5:
    case EGXAttribute::TexCoord6:
    case EGXAttribute::TexCoord7:
    {
        uint32_t texIdx = static_cast<uint32_t>(attribute) - static_cast<uint32_t>(EGXAttribute::TexCoord0);
        TexCoordIndex[texIdx] = index;
        break;
    }
    default:
        break;
    }
}

/* CVertexData */

CVertexData::CVertexData() {

}

CVertexData::~CVertexData() {

}

bool CVertexData::AttributeContainsValue(EGXAttribute attribute, const glm::vec4& value) {
    if (mVertexData.find(attribute) == mVertexData.end()) {
        return false;
    }

    const auto& attributeValues = mVertexData[attribute];
    return std::find(attributeValues.begin(), attributeValues.end(), value) != attributeValues.end();
}

uint16_t CVertexData::GetIndexOfValueInAttribute(EGXAttribute attribute, const glm::vec4& value) {
    if (mVertexData.find(attribute) == mVertexData.end()) {
        return UINT16_MAX;
    }

    const auto& attributeValues = mVertexData[attribute];
    const auto& valueItr = std::find(attributeValues.begin(), attributeValues.end(), value);

    if (valueItr == attributeValues.end()) {
        return UINT16_MAX;
    }

    return valueItr - attributeValues.begin();
}

uint16_t CVertexData::AddValueToAttribute(EGXAttribute attribute, const glm::vec4& value) {
    mVertexData[attribute].push_back(value);
    return mVertexData[attribute].size() - 1;
}

void CVertexData::BuildConverterPrimitive(const std::map<EGXAttribute, std::vector<glm::vec4>>& attributes,
    const std::vector<uint16_t>& indices, const std::vector<glm::vec4>& jointIndices,
    const std::vector<glm::vec4>& jointWeights, std::shared_ptr<SPrimitive> primitive) {

    for (uint32_t i = 0; i < indices.size(); i++) {
        uint16_t vertexIndex = indices[i];
        std::shared_ptr<SVertex> vtx = std::make_shared<SVertex>();

        if (attributes.find(EGXAttribute::NBT) != attributes.end() && attributes.at(EGXAttribute::NBT).size() != 0) {
            vtx->bUseNBT = true;
        }

        if (jointIndices.size() != 0) {
            for (uint32_t skinIndex = 0; skinIndex < 4; skinIndex++) {
                if (jointWeights[vertexIndex][skinIndex] != 0.0f) {
                    //vtx->SkinInfo.JointIndices.push_back(jointIndices[vertexIndex][skinIndex]);
                    //vtx->SkinInfo.Weights.push_back(jointWeights[vertexIndex][skinIndex]);
                }
            }
        }
        else {
            //vtx->SkinInfo.JointIndices.push_back(0);
            //vtx->SkinInfo.Weights.push_back(1.0f);
        }

        // Strip vertex attributes to only unique values, and set the vertex indices to match
        for (const auto& [attribute, values] : attributes) {
            if (attribute == EGXAttribute::NBT) {
                ProcessNBTData(values, vertexIndex, vtx);
                continue;
            }

            uint16_t newIndex = GetIndexOfValueInAttribute(attribute, values[vertexIndex]);

            if (newIndex == UINT16_MAX) {
                newIndex = AddValueToAttribute(attribute, values[vertexIndex]);
            }

            vtx->SetIndex(attribute, newIndex);
        }

        primitive->mVertices.push_back(vtx);
    }
}

void CVertexData::ProcessNBTData(const std::vector<glm::vec4>& tangents, const uint16_t vertexIndex, std::shared_ptr<SVertex> vertex) {
    glm::vec4 normal = mVertexData.at(EGXAttribute::Normal)[vertex->NormalIndex];
    glm::vec4 tangent = tangents[vertexIndex];

    glm::vec3 bitangent = glm::cross(
        glm::vec3(normal.x, normal.y, normal.z),
        glm::vec3(tangent.x, tangent.y, tangent.z)
    ) * tangent.w;

    std::shared_ptr<SNBTData> nbt = std::make_shared<SNBTData>(normal, tangent, bitangent);
    const auto itr = std::find_if(mNBTData.begin(), mNBTData.end(), [nbt](std::shared_ptr<SNBTData> const& i) { return *i.get() == *nbt.get(); });

    if (itr == mNBTData.end()) {
        vertex->NormalIndex = mNBTData.size();
        mNBTData.push_back(nbt);

        return;
    }

    vertex->NormalIndex = itr - mNBTData.begin();
}

void CVertexData::WriteVTX1(bStream::CStream& stream) {
    size_t streamStartPos = stream.tell();

    stream.writeUInt32(0x56545831);
    stream.writeUInt32(0);
    stream.writeUInt32(0x40);

    // Offsets to optional attributes
    for (uint32_t i = 0; i < 13; i++) {
        stream.writeUInt32(0);
    }

    // Attribute storage definitions
    for (const auto& [attribute, values] : mVertexData) {
        uint32_t componentCount = 0, componentType = 0;
        uint8_t fixedPointExponent = 0;

        switch (attribute) {
            case EGXAttribute::Position:
                componentCount = static_cast<uint32_t>(EGXComponentCount::Position_XYZ);
                componentType = static_cast<uint32_t>(EGXComponentType::Float);
                fixedPointExponent = 0;
                break;
            case EGXAttribute::Normal:
                componentCount = static_cast<uint32_t>(EGXComponentCount::Normal_XYZ);
                componentType = static_cast<uint32_t>(EGXComponentType::Signed16);
                fixedPointExponent = FIXED_POINT_EXP_NORMAL;
                break;
            case EGXAttribute::Color0:
            case EGXAttribute::Color1:
                componentCount = static_cast<uint32_t>(EGXComponentCount::Color_RGBA);
                componentType = static_cast<uint32_t>(EGXComponentType::RGBA8);
                fixedPointExponent = 0;
                break;
            case EGXAttribute::TexCoord0:
            case EGXAttribute::TexCoord1:
            case EGXAttribute::TexCoord2:
            case EGXAttribute::TexCoord3:
            case EGXAttribute::TexCoord4:
            case EGXAttribute::TexCoord5:
            case EGXAttribute::TexCoord6:
            case EGXAttribute::TexCoord7:
                componentCount = static_cast<uint32_t>(EGXComponentCount::TexCoord_UV);
                componentType = static_cast<uint32_t>(EGXComponentType::Signed16);
                fixedPointExponent = FIXED_POINT_EXP_TEXCOORD;
                break;
            default:
                break;
        }

        stream.writeUInt32(static_cast<uint32_t>(attribute));
        stream.writeUInt32(componentCount);
        stream.writeUInt32(componentType);
        stream.writeUInt8(fixedPointExponent);

        stream.writeUInt8(UINT8_MAX);
        stream.writeUInt16(UINT16_MAX);
    }

    // Add null attribute
    stream.writeUInt32(static_cast<uint32_t>(EGXAttribute::Null));
    stream.writeUInt32(static_cast<uint32_t>(EGXComponentCount::Position_XYZ));
    stream.writeUInt32(0);
    stream.writeUInt8(0);

    stream.writeUInt8(UINT8_MAX);
    stream.writeUInt16(UINT16_MAX);

    // Pad attribute list to 16 bytes
    Util::PadStreamWithString(&stream, 16);

    // Attribute values
    for (const auto& [attribute, values] : mVertexData) {
        size_t currentStreamPos = stream.tell();

        uint32_t attributeOffset = 0;
        switch (attribute) {
            case EGXAttribute::Position:
            {
                attributeOffset = 0x0C;
                break;
            }
            case EGXAttribute::Normal:
            {
                attributeOffset = 0x10;
                break;
            }
            case EGXAttribute::NBT:
            {
                attributeOffset = 0x14;
                break;
            }
            case EGXAttribute::Color0:
            case EGXAttribute::Color1:
            {
                uint32_t colorIndex = static_cast<uint32_t>(attribute) - static_cast<uint32_t>(EGXAttribute::Color0);
                attributeOffset = 0x18 + colorIndex * 4;

                break;
            }
            case EGXAttribute::TexCoord0:
            case EGXAttribute::TexCoord1:
            case EGXAttribute::TexCoord2:
            case EGXAttribute::TexCoord3:
            case EGXAttribute::TexCoord4:
            case EGXAttribute::TexCoord5:
            case EGXAttribute::TexCoord6:
            case EGXAttribute::TexCoord7:
            {
                uint32_t texCoordIndex = static_cast<uint32_t>(attribute) - static_cast<uint32_t>(EGXAttribute::TexCoord0);
                attributeOffset = 0x20 + texCoordIndex * 4;

                break;
            }
            default:
            {
                break;
            }
        }

        stream.seek(streamStartPos + attributeOffset);
        stream.writeUInt32(static_cast<uint32_t>(currentStreamPos - streamStartPos));
        stream.seek(currentStreamPos);

        for (const glm::vec4& value : values) {
            switch (attribute) {
                case EGXAttribute::Position:
                    stream.writeFloat(value.x);
                    stream.writeFloat(value.y);
                    stream.writeFloat(value.z);

                    continue;
                case EGXAttribute::Normal:
                    stream.writeInt16(static_cast<int16_t>(value.x / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));
                    stream.writeInt16(static_cast<int16_t>(value.y / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));
                    stream.writeInt16(static_cast<int16_t>(value.z / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));

                    continue;
                case EGXAttribute::Color0:
                case EGXAttribute::Color1:
                    stream.writeUInt8(static_cast<uint8_t>(value.x * 255.0f));
                    stream.writeUInt8(static_cast<uint8_t>(value.y * 255.0f));
                    stream.writeUInt8(static_cast<uint8_t>(value.z * 255.0f));
                    stream.writeUInt8(static_cast<uint8_t>(value.w * 255.0f));

                    continue;
                case EGXAttribute::TexCoord0:
                case EGXAttribute::TexCoord1:
                case EGXAttribute::TexCoord2:
                case EGXAttribute::TexCoord3:
                case EGXAttribute::TexCoord4:
                case EGXAttribute::TexCoord5:
                case EGXAttribute::TexCoord6:
                case EGXAttribute::TexCoord7:
                    stream.writeInt16(static_cast<int16_t>(value.x / std::powf(0.5f, FIXED_POINT_EXP_TEXCOORD)));
                    stream.writeInt16(static_cast<int16_t>(value.y / std::powf(0.5f, FIXED_POINT_EXP_TEXCOORD)));

                    continue;
                default:
                    continue;
            }
        }

        // Pad section to 32 bytes
        Util::PadStreamWithString(&stream, 32);
    }

    // Write NBT data if present. Not handled above because it's ~*~special~*~
    if (mNBTData.size() != 0) {
        size_t currentStreamPos = stream.tell();

        // Write NBT offset. This is the only(?) way that J3D knows this data exists.
        // It's not technically listed in the attribute table.
        stream.seek(streamStartPos + 0x14);
        stream.writeUInt32(static_cast<uint32_t>(currentStreamPos - streamStartPos));
        stream.seek(currentStreamPos);

        WriteNBTData(stream);

        // Pad section to 32 bytes
        Util::PadStreamWithString(&stream, 32);
    }

    size_t streamEndPos = stream.tell();

    // Write section size
    stream.seek(streamStartPos + 4);
    stream.writeUInt32(static_cast<uint32_t>(streamEndPos - streamStartPos));
    stream.seek(streamEndPos);
}

void CVertexData::WriteNBTData(bStream::CStream& stream) {
    for (const auto& nbt : mNBTData) {
        stream.writeInt16(static_cast<int16_t>(nbt->Normal.x / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));
        stream.writeInt16(static_cast<int16_t>(nbt->Normal.y / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));
        stream.writeInt16(static_cast<int16_t>(nbt->Normal.z / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));

        stream.writeInt16(static_cast<int16_t>(nbt->Tangent.x / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));
        stream.writeInt16(static_cast<int16_t>(nbt->Tangent.y / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));
        stream.writeInt16(static_cast<int16_t>(nbt->Tangent.z / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));

        stream.writeInt16(static_cast<int16_t>(nbt->Bitangent.x / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));
        stream.writeInt16(static_cast<int16_t>(nbt->Bitangent.y / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));
        stream.writeInt16(static_cast<int16_t>(nbt->Bitangent.z / std::powf(0.5f, FIXED_POINT_EXP_NORMAL)));
    }

    // Pad section to 32 bytes
    Util::PadStreamWithString(&stream, 32);
}
