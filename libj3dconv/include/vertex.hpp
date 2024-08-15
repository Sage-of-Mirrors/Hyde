#pragma once

#include "types.hpp"
#include "j3denum.hpp"

#include <glm/glm.hpp>

#include <map>
#include <vector>

struct SPrimitive;

struct SVertex {
    uint16_t PositionIndex = UINT16_MAX;

    // If this is set, then the mesh is using normal/binormal/tangent
    // rather than typical vertex normals.
    bool bUseNBT = false;
    uint16_t NormalIndex = UINT16_MAX;

    uint16_t ColorIndex[2]{ UINT16_MAX, UINT16_MAX };
    uint16_t TexCoordIndex[8]{
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX,
        UINT16_MAX
    };

    uint16_t PosMatrixIndex = UINT16_MAX;

    std::vector<uint16_t> JointIndices;
    std::vector<float> Weights;

    void SetIndex(EGXAttribute attribute, uint16_t index);
};

struct SNBTData {
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;

    SNBTData(glm::vec3 nrm, glm::vec3 tan, glm::vec3 bit) : Normal(nrm), Tangent(tan), Bitangent(bit) { }

    bool operator==(const SNBTData& other) {
        if (Normal == other.Normal && Tangent == other.Tangent && Bitangent == other.Bitangent) {
            return true;
        }

        return false;
    }

    bool operator!= (const SNBTData& other) {
        return !(operator==(other));
    }
};

class CVertexData {
    std::map<EGXAttribute, std::vector<glm::vec4>> mVertexData;
    shared_vector<SNBTData> mNBTData;

    void ProcessNBTData(const std::vector<glm::vec4>& tangents, const uint16_t vertexIndex, std::shared_ptr<SVertex> vertex);
    void WriteNBTData(bStream::CStream& stream);

public:
    CVertexData();
    ~CVertexData();

    bool AttributeContainsValue(EGXAttribute attribute, const glm::vec4& value);
    uint16_t AddValueToAttribute(EGXAttribute attribute, const glm::vec4& value);
    uint16_t GetIndexOfValueInAttribute(EGXAttribute attribute, const glm::vec4& value);

    void BuildConverterPrimitive(const std::map<EGXAttribute, std::vector<glm::vec4>>& attributes,
        const std::vector<uint16_t>& indices,
        const std::vector<glm::vec4>& jointIndices,
        const std::vector<glm::vec4>& jointWeights,
        std::shared_ptr<SPrimitive> primitive);

    void WriteVTX1(bStream::CStream& stream);

    uint32_t GetVertexCount() const { return static_cast<uint32_t>(mVertexData.at(EGXAttribute::Position).size()); }
};
