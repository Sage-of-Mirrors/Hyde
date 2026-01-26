#pragma once

#include "types.hpp"
#include "util.hpp"
#include "j3denum.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct ufbx_scene;
struct ufbx_mesh;
struct ufbx_mesh_part;

struct SVertex;
class CVertexData;

/* SPrimitive */

struct SPrimitive {
    EGXPrimitiveType mPrimitiveType = EGXPrimitiveType::None;
    std::vector<SVertex*> mVertices;

public:
    SPrimitive() { }
    ~SPrimitive();
};

/* CShape */

class CShape {
public:
    CShape();
    ~CShape();

    void CalculateBoundingVolume(const std::vector<glm::vec4>& positions);

    void AddPrimitive(SPrimitive& prim) { mPrimitives.push_back(prim); }
    std::vector<SPrimitive>& GetPrimitives() { return mPrimitives; }

    const std::string& GetMaterialName() const { return mMaterialName; }
    uint32_t GetIndex() const { return mIndex; }
    uint32_t GetMaterialIndex() const { return mMaterialIndex; }
    uint32_t GetJointIndex() const { return mJointIndex; }

    void SetMaterialName(std::string name) { mMaterialName = name; }
    void SetIndex(uint32_t index) { mIndex = index; }
    void SetMaterialIndex(uint32_t index) { mMaterialIndex = index; }
    void SetJointIndex(uint32_t index) { mJointIndex = index; }

    bool GetAttributeEnabled(EGXAttribute attr) const;
    void SetAttributeEnabled(EGXAttribute attr);

private:
    std::vector<SPrimitive> mPrimitives;

    // Utility properties
    std::string mMaterialName = "";

    uint32_t mIndex = UINT32_MAX;
    uint32_t mMaterialIndex = UINT32_MAX;
    uint32_t mJointIndex = UINT32_MAX;

    // J3D properties
    uint8_t mMatrixType;

    bool mHasPosition : 1;
    bool mHasNormals : 1;
    bool mHasColor0 : 1;
    bool mHasColor1 : 1;
    bool mHasTexCoord0 : 1;
    bool mHasTexCoord1 : 1;
    bool mHasTexCoord2 : 1;
    bool mHasTexCoord3 : 1;
    bool mHasTexCoord4 : 1;
    bool mHasTexCoord5 : 1;
    bool mHasTexCoord6 : 1;
    bool mHasTexCoord7 : 1;
    bool mHasPosMtx : 1;

    std::vector<glm::vec3> mPositions;
    std::vector<glm::vec3> mNormals;
    std::vector<glm::vec4> mColors[2];
    std::vector<glm::vec2> mTexCoords[8];

    Util::UConvBoundingVolume mBounds;
};

/* UConverterShape Data */

class CShapeData
{
public:
    CShapeData() = default;
    ~CShapeData();

    std::vector<CShape*>& GetShapes() { return mShapes; }

    void ProcessMeshes(ufbx_scene* scene);

private:
    CShape* CreateShapeFromMeshPart(const ufbx_mesh* mesh, const ufbx_mesh_part& partInfo);

    std::vector<CShape*> mShapes;
};
