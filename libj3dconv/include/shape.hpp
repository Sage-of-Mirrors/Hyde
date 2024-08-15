#pragma once

#include "types.hpp"
#include "util.hpp"
#include "j3denum.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct SVertex;
class CVertexData;

/* SPrimitive */

struct SPrimitive {
    EGXPrimitiveType mPrimitiveType = EGXPrimitiveType::None;
    shared_vector<SVertex> mVertices;

public:
    SPrimitive() { }
    ~SPrimitive();
};

/* CShape */

class CShape {
    shared_vector<SPrimitive> mPrimitives;

    // Utility properties
    std::string mMaterialName = "";

    uint32_t mIndex = UINT32_MAX;
    uint32_t mMaterialIndex = UINT32_MAX;
    uint32_t mJointIndex = UINT32_MAX;

    // J3D properties
    uint8_t mMatrixType;
    std::vector<EGXAttribute> mEnabledAttributes;
    Util::UConvBoundingVolume mBounds;

public:
    CShape();
    ~CShape();

    void CalculateBoundingVolume(const std::vector<glm::vec4>& positions);

    void AddPrimitive(std::shared_ptr<SPrimitive> prim) { if (prim != nullptr) mPrimitives.push_back(prim); }
    shared_vector<SPrimitive>& GetPrimitives() { return mPrimitives; }

    const std::string& GetMaterialName() const { return mMaterialName; }

    uint32_t GetIndex() const { return mIndex; }
    uint32_t GetMaterialIndex() const { return mMaterialIndex; }
    uint32_t GetJointIndex() const { return mJointIndex; }

    void SetMaterialName(std::string name) { mMaterialName = name; }

    void SetIndex(uint32_t index) { mIndex = index; }
    void SetMaterialIndex(uint32_t index) { mMaterialIndex = index; }
    void SetJointIndex(uint32_t index) { mJointIndex = index; }
};

/* UConverterShape Data */

class CShapeData {
    shared_vector<CShape> mShapes;

    void ReadGltfVertexAttribute(
        const tinygltf::Model* model,
        std::vector<bStream::CMemoryStream>& buffers,
        uint32_t attributeAccessorIndex,
        std::vector<glm::vec4>& values
    );
    void ReadGltfIndices(
        const tinygltf::Model* model,
        std::vector<bStream::CMemoryStream>& buffers,
        uint32_t indexAccessorIndex,
        std::vector<uint16_t>& indices
    );

public:
    CShapeData();
    ~CShapeData();

    void BuildVertexData(
        tinygltf::Model* model,
        CVertexData& vertexData,
        std::vector<bStream::CMemoryStream>& buffers
    );

    shared_vector<CShape>& GetShapes() { return mShapes; }
};